#pragma once
#include <map>
#include <vector>

#include "BaseComponent.h"

constexpr int STEPS = 11;

class TrashTheCacheComponent : public BaseComponent
{
public:
	TrashTheCacheComponent() = default;
	~TrashTheCacheComponent() override;
	void Update() override;
	void FixedUpdate() override;
	void Init() override;
	void Render() const override;
	void RenderGUI() override;


protected:
	void ClearAverage();
	int m_Sample;
	float m_Step[STEPS]{1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
	float m_AverageResults[STEPS];
	bool m_IsComputing;

private:
	void ComputeResult();
};
