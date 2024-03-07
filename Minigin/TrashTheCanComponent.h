#pragma once
#include <map>
#include <vector>

#include "BaseComponent.h"

class TrashTheCanComponent : public BaseComponent
{
public:
	TrashTheCanComponent() = default;
	~TrashTheCanComponent() override;
	void Update() override;
	void FixedUpdate() override;
	void Init() override;
	void Render() const override;

private:
	void ClearAverage();
	void Exercise01();
	void Exercise02();
	int m_Sample;
	float m_Step[11]{1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
	float m_AverageResults[11];
	bool m_IsComputing;
};
