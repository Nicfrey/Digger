#pragma once
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
		GameObject3DAlt() : m_Transform{ new TransformAlt{} }, m_ID{} {};
		~GameObject3DAlt()
		{
			delete m_Transform;
			m_Transform = nullptr;
		};
		TransformAlt* m_Transform;
		int m_ID;
	};
};

