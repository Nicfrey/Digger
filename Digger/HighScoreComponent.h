#pragma once
#include <string>

#include "BaseComponent.h"

class HighScoreComponent : public BaseComponent
{
public:
	HighScoreComponent(bool displayTheScore = false);
	std::shared_ptr<BaseComponent> Clone() const override;
	void Init() override;
	void DisplayTheScore(bool display);
private:
	void SaveToJson(const std::string& name, int score,const std::string& filename);
	void DisplayTheTopFiveScore();
	void DisplayInputScore();
	void UpdateUI();
	void SaveHighScore(const std::string& name);
	std::string m_Text;
	bool m_DisplayTheScore{ false };

};
 
