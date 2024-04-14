#pragma once
#include <chrono>
#include <map>

#include "TrashTheCacheComponent.h"

class TrashTheCacheGOComponent : public TrashTheCacheComponent
{
public:
	TrashTheCacheGOComponent() = default;
	~TrashTheCacheGOComponent() override = default;
	void Update() override;
	void FixedUpdate() override;
	void Init() override;
	void Render() const override;
	void RenderGUI() override;

private:
	void ClearAverageAlt();
	template<typename T>
	void ComputeResult(); // Issue when initialize arr for alt
	void ComputeResult3D();
	void ComputeResult3DAlt();

	float m_AverageResultsAlt[STEPS];
	bool m_IsComputingAlt;

	struct TransformAlt
	{
		float matrix[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};
	};

	class GameObject3D final
	{
	public:
		GameObject3D() = default;
		TransformAlt m_Transform;
		int m_ID;
	};

	class GameObject3DAlt final
	{
	public:
		GameObject3DAlt() : m_Transform{ nullptr }, m_ID{} {}
		~GameObject3DAlt()
		{
			delete m_Transform;
			m_Transform = nullptr;
		}
		TransformAlt* m_Transform;
		int m_ID;
	};
};

template<typename T>
void TrashTheCacheGOComponent::ComputeResult()
{
	if(m_Sample == 0)
	{
		m_IsComputing = false;
		return;
	}

	std::map<int, std::vector<float>> timeCalculation{};
	const int length{ static_cast<int>(pow(2,26)) };
	for(int j{}; j < m_Sample; ++j)
	{
		std::vector<float> calculations{};
		T* arr{ new T[length] };
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
		delete[] arr;
		arr = nullptr;
	}

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

