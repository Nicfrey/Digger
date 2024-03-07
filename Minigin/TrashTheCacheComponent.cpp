#include "TrashTheCacheComponent.h"

#include <chrono>
#include <future>
#include <iostream>
#include <thread>

#include "imgui_plot.h"
#include "implot.h"
#include "Renderer.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"


TrashTheCacheComponent::~TrashTheCacheComponent()
{

}

void TrashTheCacheComponent::Update()
{
}

void TrashTheCacheComponent::FixedUpdate()
{
}

void TrashTheCacheComponent::Init()
{
}

void TrashTheCacheComponent::Render() const
{
	
}

void TrashTheCacheComponent::RenderGUI()
{
	bool windowActive{ true };
	ImGui::Begin("Exercice 1", &windowActive);
	ImGui::InputInt("sample", &m_Sample);
	if (ImGui::Button("Trash The cache"))
	{
		ClearAverage();
		m_IsComputing = true;
		std::thread asyncThread{ &TrashTheCacheComponent::ComputeResult, this };
		asyncThread.detach();
	}
	if (m_IsComputing)
	{
		ImGui::Text("Wait for it");
	}
	else if (!m_IsComputing && m_AverageResults[0] != 0.f)
	{
		ImGui::PlotConfig conf;
		conf.values.xs = m_Step;
		conf.values.ys = m_AverageResults;
		conf.values.count = 11;
		conf.scale.max = 100.f;
		conf.scale.min = -1.f;
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%.2f, y=%.2f";
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(400.f, 400.f);
		conf.line_thickness = 2.f;
		ImGui::Plot("Result", conf);
	}

	ImGui::End();
}

void TrashTheCacheComponent::ClearAverage()
{
	for(int i{}; i < STEPS; ++i)
	{
		m_AverageResults[i] = 0;
	}
}

void TrashTheCacheComponent::ComputeResult()
{
	if(m_Sample == 0)
	{
		m_IsComputing = false;
		return;
	}
	std::map<int, std::vector<float>> timeCalculation{};
	const int length{static_cast<int>(pow(2, 26))};
	for (int j{}; j < m_Sample; ++j)
	{
		std::vector<float> calculations{};
		int* arr{new int[length]};
		for (int step{1}; step <= 1024; step *= 2)
		{
			auto start{std::chrono::high_resolution_clock::now()};
			for (int i{}; i < length; i += step)
			{
				arr[i] *= 2;
			}
			auto end{std::chrono::high_resolution_clock::now()};
			const auto elapsedTime{std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()};
			calculations.emplace_back(elapsedTime / 1000.f);
		}
		timeCalculation.emplace(j, calculations);
		delete[] arr;
	}
	const int divider{ m_Sample - 2 };
	// Remove the min and max for each
	for (int i{}; i < STEPS; ++i)
	{
		// Go through 1 - 2 - 4 - 8 - 16 - 32 - 64 - 128 - 256 - 512 - 1024
		float max{FLT_MIN};
		float min{FLT_MAX};
		for (const auto& pair : timeCalculation)
		{
			// Go through every sample
			auto result{pair.second};
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
