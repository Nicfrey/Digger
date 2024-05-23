#pragma once
#include "Singleton.h"
#include "Blackboard.h"

/**
 * \brief Game instance that stores data that can be shared between levels
 */
class GameInstance : public dae::Singleton<GameInstance>
{
public:
	template<typename T>
	bool GetValue(const std::string& key, T& data);
	template<typename T>
	bool AddValue(const std::string& key, T& data);
	template<typename T>
	bool ChangeValue(const std::string& key, T data);
private:
	Blackboard m_pGameInstance;
};

template <typename T>
bool GameInstance::GetValue(const std::string& key, T& data)
{
	return m_pGameInstance.GetValue(key,data);
}

template <typename T>
bool GameInstance::AddValue(const std::string& key, T& data)
{
	return m_pGameInstance.AddValue(key, data);
}

template <typename T>
bool GameInstance::ChangeValue(const std::string& key, T data)
{
	return m_pGameInstance.ChangeValue(key, data);
}
