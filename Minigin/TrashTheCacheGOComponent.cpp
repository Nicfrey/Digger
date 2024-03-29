#include "TrashTheCacheGOComponent.h"

#include <map>
#include <thread>

#include "imgui_plot.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"


void TrashTheCacheGOComponent::Update()
{
}

void TrashTheCacheGOComponent::FixedUpdate()
{
	TrashTheCacheComponent::FixedUpdate();
}

void TrashTheCacheGOComponent::Init()
{
	TrashTheCacheComponent::Init();
}

void TrashTheCacheGOComponent::Render() const
{
	TrashTheCacheComponent::Render();
}

void TrashTheCacheGOComponent::RenderGUI()
{

	ImGui::PlotConfig conf;
	conf.values.count = 11;
	conf.scale.min = -1.f;
	conf.tooltip.show = true;
	conf.tooltip.format = "x=%.2f, y=%.2f";
	conf.grid_x.show = true;
	conf.grid_y.show = true;
	conf.frame_size = ImVec2(200.f, 100.f);
	conf.line_thickness = 2.f;

	bool windowActive{ true };
	ImGui::Begin("Exercice 2", &windowActive);
	ImGui::InputInt("sample", &m_Sample);
	if (!m_IsComputing && ImGui::Button("Trash The can GameObject3D"))
	{
		ClearAverage();
		m_IsComputing = true;
		std::thread asyncThread{ &TrashTheCacheGOComponent::ComputeResult3D, this };
		asyncThread.detach();
		if(asyncThread.joinable())
		{
			asyncThread.join();
		}
	}
	if (m_IsComputing)
	{
		ImGui::Text("Wait for it");
	}
	else if (!m_IsComputing && m_AverageResults[0] != 0.f)
	{
		const ImU32 color{ ImColor{124,252,0} };
		conf.values.xs = m_Step;
		conf.values.ys = m_AverageResults;
		conf.values.color = color;
		conf.scale.max = m_AverageResults[0];
		ImGui::Plot("ResultAlt", conf);
	}

	if (!m_IsComputingAlt && ImGui::Button("Trash The Can GameObject3DAlt"))
	{
		ClearAverageAlt();
		m_IsComputingAlt = true;
		std::thread asyncThread{ &TrashTheCacheGOComponent::ComputeResult3DAlt, this };
		asyncThread.detach();
		if (asyncThread.joinable())
		{
			asyncThread.join();
		}
	}
	if (m_IsComputingAlt)
	{
		ImGui::Text("Wait for it");
	}
	else if (!m_IsComputingAlt && m_AverageResultsAlt[0] != 0.f)
	{
		const ImU32 color{ ImColor{135,206,235} };
		conf.values.xs = m_Step;
		conf.values.ys = m_AverageResultsAlt;
		conf.values.color = color;
		conf.scale.max = m_AverageResultsAlt[0];
		ImGui::Plot("ResultCombined", conf);
	}

	if (m_AverageResultsAlt[0] != 0.f && m_AverageResults[0] != 0.f)
	{
		ImGui::Text("Combined:");
		const float max{ std::max(m_AverageResultsAlt[0],m_AverageResults[0]) };
		const float** listAverage{ new const float*[2]{m_AverageResults, m_AverageResultsAlt} };
		const ImU32 color[2]{ ImColor{ 124,252,0 },ImColor{135,206,235} };
		conf.values.xs = m_Step;
		conf.values.ys = nullptr;
		conf.values.ys_list = listAverage;
		conf.values.ys_count = 2;
		conf.scale.max = max;
		conf.values.colors = color;
		ImGui::Plot("ResultCombined", conf);
		delete[] listAverage;
	}

	ImGui::End();
}

void TrashTheCacheGOComponent::ClearAverageAlt()
{
	for(int i{}; i < STEPS; ++i)
	{
		m_AverageResultsAlt[i] = 0.f;
	}
}

void TrashTheCacheGOComponent::ComputeResult3D()
{
	if (m_Sample == 0)
	{
		m_IsComputing = false;
		return;
	}

	std::map<int, std::vector<float>> timeCalculation{};
	const int length{ static_cast<int>(pow(2,26)) };
	GameObject3D* arr{ new GameObject3D[length]{} };
	for (int j{}; j < m_Sample; ++j)
	{
		std::vector<float> calculations{};
		for (int step{ 1 }; step <= 1024; step *= 2)
		{
			auto start{ std::chrono::high_resolution_clock::now() };
			for (int i{}; i < length; i += step)
			{
				arr[i].m_ID *= 2;
			}
			auto end{ std::chrono::high_resolution_clock::now() };
			const auto elapsedTime{ std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() };
			calculations.emplace_back(elapsedTime / 1000.f);
		}
		timeCalculation.emplace(j, calculations);
	}
	delete[] arr;
	arr = nullptr;

	const int divider{ m_Sample - 2 };
	// Remove the min and max for each
	for (int i{}; i < STEPS; ++i)
	{
		// Go through 1 - 2 - 4 - 8 - 16 - 32 - 64 - 128 - 256 - 512 - 1024
		float max{ FLT_MIN };
		float min{ FLT_MAX };
		for (const auto& pair : timeCalculation)
		{
			// Go through every sample
			auto result{ pair.second };
			m_AverageResults[i] += result[i];
			max = std::max(max, result[i]);
			min = std::min(min, result[i]);
		}
		m_AverageResults[i] -= max;
		m_AverageResults[i] -= min;
		m_AverageResults[i] /= divider;
	}
	m_IsComputing = false;
}

void TrashTheCacheGOComponent::ComputeResult3DAlt()
{
	if (m_Sample == 0)
	{
		m_IsComputingAlt = false;
		return;
	}

	std::map<int, std::vector<float>> timeCalculation{};
	const int length{ static_cast<int>(pow(2,26)) };
	GameObject3DAlt* arr{ new GameObject3DAlt[length]{} };
	for (int j{}; j < m_Sample; ++j)
	{
		std::vector<float> calculations{};
		for (int step{ 1 }; step <= 1024; step *= 2)
		{
			auto start{ std::chrono::high_resolution_clock::now() };
			for (int i{}; i < length; i += step)
			{
				arr[i].m_ID *= 2;
			}
			auto end{ std::chrono::high_resolution_clock::now() };
			const auto elapsedTime{ std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() };
			calculations.emplace_back(elapsedTime / 1000.f);
		}
		timeCalculation.emplace(j, calculations);
	}
	delete[] arr;
	arr = nullptr;

	const int divider{ m_Sample - 2 };
	// Remove the min and max for each
	for (int i{}; i < STEPS; ++i)
	{
		// Go through 1 - 2 - 4 - 8 - 16 - 32 - 64 - 128 - 256 - 512 - 1024
		float max{ FLT_MIN };
		float min{ FLT_MAX };
		for (const auto& pair : timeCalculation)
		{
			// Go through every sample
			auto result{ pair.second };
			m_AverageResultsAlt[i] += result[i];
			max = std::max(max, result[i]);
			min = std::min(min, result[i]);
		}
		m_AverageResultsAlt[i] -= max;
		m_AverageResultsAlt[i] -= min;
		m_AverageResultsAlt[i] /= divider;
	}
	m_IsComputingAlt = false;
}
