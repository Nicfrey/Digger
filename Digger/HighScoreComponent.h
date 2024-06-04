#pragma once
#include <string>

#include "BaseComponent.h"

class HighScoreComponent : public BaseComponent
{
public:
	HighScoreComponent() = default;
	void SaveHighScore(const std::string& name);
private:
	void SaveToJson(const std::string& name, int score,const std::string& filename);
};

