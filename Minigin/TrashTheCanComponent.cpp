#include "TrashTheCanComponent.h"

#include <chrono>
#include <future>
#include <iostream>
#include <thread>

#include "imgui_plot.h"
#include "implot.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"


TrashTheCanComponent::~TrashTheCanComponent()
{
}

void TrashTheCanComponent::Update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	bool windowActive{ true };

	ImGui::Begin("Exercice 1", &windowActive);
	ImGui::InputInt("sample", &m_Sample);
	if (ImGui::Button("Trash The can"))
	{
		m_IsComputing = true;
		std::thread asyncThread{ &TrashTheCanComponent::Exercise01, this };
		asyncThread.detach();
	}
	if(m_IsComputing)
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
		ImGui::Plot("Salut", conf);
		ImGui::PlotLines("Result", m_AverageResults, 11, 0, nullptr, -1.0f, -1.0f, ImVec2(100, 100));
		ImGui::Text("I'm full");
	}

	ImGui::End();
}

void TrashTheCanComponent::FixedUpdate()
{
}

void TrashTheCanComponent::Init()
{
}

void TrashTheCanComponent::Render() const
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void TrashTheCanComponent::Exercise01()
{
	std::map<int, std::vector<float>> timeCalculation{};
	const int length{ static_cast<int>(pow(2,26)) };
	for(int j{}; j <= m_Sample; ++j)
	{
		std::vector<float> calculations{};
		int* arr{ new int[length] };
		for (int step{ 1 }; step <= 1024; step *= 2)
		{
			auto start{ std::chrono::high_resolution_clock::now() };
			for (int i{}; i < length; i += step)
			{
				arr[i] *= 2;
			}
			auto end{ std::chrono::high_resolution_clock::now() };
			const auto elapsedTime{ std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() };
			calculations.emplace_back(elapsedTime / 1000.f);
		}
		timeCalculation.emplace(j, calculations);
		delete[] arr;
	}

	for(const auto& pair: timeCalculation)
	{
		auto result{ pair.second };
		for(int i{}; i < m_Sample; ++i)
		{
			m_AverageResults[i] += result[i];
		}
	}
	for (int i{}; i < m_Sample; ++i)
	{
		m_AverageResults[i] /= m_Sample;
	}

	m_IsComputing = false;
}

void TrashTheCanComponent::Exercise02()
{
}
