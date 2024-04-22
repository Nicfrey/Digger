#include "Blackboard.h"

Blackboard::~Blackboard()
{
	for(auto& data : m_BlackboardData)
	{
		delete data.second;
		data.second = nullptr;
	}
	m_BlackboardData.clear();
}
