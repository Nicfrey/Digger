#pragma once
#include <memory>
#include <string>
#include <unordered_map>

class IBlackboardField
{
public:
	IBlackboardField() = default;
	virtual ~IBlackboardField() = default;
	IBlackboardField(const IBlackboardField& other) = delete;
	IBlackboardField(IBlackboardField&& other) noexcept = delete;
	IBlackboardField& operator=(const IBlackboardField& other) = delete;
	IBlackboardField& operator=(IBlackboardField&& other) noexcept = delete;
};

template<typename T>
class BlackboardField : public IBlackboardField
{
public:
	explicit BlackboardField(T data);
	~BlackboardField() override = default;
	BlackboardField(const BlackboardField& other) = delete;
	BlackboardField(BlackboardField&& other) noexcept = delete;
	BlackboardField& operator=(const BlackboardField& other) = delete;
	BlackboardField& operator=(BlackboardField&& other) noexcept = delete;
	T GetData() const;
	void SetData(T data);
private:
	T m_Data;
};

class Blackboard
{
public:
	Blackboard() = default;
	~Blackboard() = default;
	template<typename T>
	bool GetValue(const std::string& key, T& data);
	template<typename T>
	bool AddValue(const std::string& key, T data);
	template<typename T>
	bool ChangeValue(const std::string& key, T data);
private:
	std::unordered_map<std::string, IBlackboardField*> m_BlackboardData;
};

template <typename T>
BlackboardField<T>::BlackboardField(T data) : IBlackboardField{}, m_Data { data }
{
}

template <typename T>
T BlackboardField<T>::GetData() const
{
	return m_Data;
}

template <typename T>
void BlackboardField<T>::SetData(T data)
{
	m_Data = data;
}

template <typename T>
bool Blackboard::GetValue(const std::string& key, T& data)
{
	if (BlackboardField<T>* field{ dynamic_cast<BlackboardField<T>*>(m_BlackboardData[key]) })
	{
		data = field->GetData();
		return true;
	}
	return false;
}

template <typename T>
bool Blackboard::AddValue(const std::string& key,T data)
{
	const auto it = m_BlackboardData.find(key);
	if (it != m_BlackboardData.end())
	{
		return false;
	}
	m_BlackboardData[key] = new BlackboardField<T>(data);
	return true;
}

template <typename T>
bool Blackboard::ChangeValue(const std::string& key, T data)
{
	const auto it = m_BlackboardData.find(key);
	if (it == m_BlackboardData.end())
	{
		return false;
	}
	if (BlackboardField<T>* field{ dynamic_cast<BlackboardField<T>*>(m_BlackboardData[key]) })
	{
		field->SetData(data);
		return true;
	}
	return false;
}

