#include "TrashTheCacheGOComponent.h"

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
	bool windowActive{ true };
	ImGui::Begin("Exercice 2", &windowActive);
	ImGui::InputInt("sample", &m_Sample);
	if (ImGui::Button("Trash The can GameObject3D"))
	{
		ClearAverage();
		m_IsComputing = true;
		std::thread asyncThread{ &TrashTheCacheGOComponent::ComputeResult3D, this };
		asyncThread.detach();
	}
	if (ImGui::Button("Trash The Can GameObject3DAlt"))
	{
		ClearAverageAlt();
		m_IsComputingAlt = true;
		std::thread asyncThread{ &TrashTheCacheGOComponent::ComputeResult3DAlt, this };
		asyncThread.detach();
	}
	ImGui::PlotConfig conf;
	conf.values.count = 11;
	conf.scale.max = 100.f;
	conf.scale.min = -1.f;
	conf.tooltip.show = true;
	conf.tooltip.format = "x=%.2f, y=%.2f";
	conf.grid_x.show = true;
	conf.grid_y.show = true;
	conf.frame_size = ImVec2(100.f, 100.f);
	conf.line_thickness = 2.f;

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
		ImGui::Plot("ResultAlt", conf);
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
		ImGui::Plot("ResultCombined", conf);
	}

	if (m_AverageResultsAlt[0] != 0.f && m_AverageResults[0] != 0.f)
	{
		ImU32 color{ ImColor{135,206,235} };
		conf.values.xs = m_Step;
		conf.values.ys = m_AverageResultsAlt;
		conf.values.color = color;
		ImGui::Plot("ResultCombined", conf);
		color = ImColor{ 124,252,0 };
		conf.values.xs = m_Step;
		conf.values.ys = m_AverageResults;
		conf.values.color = color;
		ImGui::Plot("ResultCombined", conf);
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
}

void TrashTheCacheGOComponent::ComputeResult3DAlt()
{
}
