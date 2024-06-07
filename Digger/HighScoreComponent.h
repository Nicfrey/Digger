#pragma once
#include <string>

#include "BaseComponent.h"

class HighScoreComponent : public BaseComponent
{
public:
	HighScoreComponent() = default;
	std::shared_ptr<BaseComponent> Clone() const override;
	void Init() override;
	void SaveHighScore(const std::string& name);
private:
	void SaveToJson(const std::string& name, int score,const std::string& filename);
	std::string m_Text;
};

