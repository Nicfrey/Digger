#include "HighScoreComponent.h"

#include <fstream>
#include <iostream>

#include "DiggerUtils.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "json.hpp"
#include "PlayerComponent.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ScoreComponent.h"

void HighScoreComponent::SaveHighScore(const std::string& name)
{
	// Get all player object
	int score{};
	const auto& players{ dae::SceneManager::GetInstance().GetGameObjectsWithComponent<PlayerComponent>() };
	for(auto& player : players)
	{
		// Get the score of the player
		score += player->GetComponent<ScoreComponent>()->GetScore();
	}
	// Get the current game mode
	DiggerUtils::DiggerGameMode gameMode;
	GameInstance::GetInstance().GetValue("CurrentGameMode", gameMode);
	switch(gameMode)
	{
	case DiggerUtils::DiggerGameMode::SinglePlayer:
		SaveToJson(name, score, "Scores/HighScoreSinglePlayer.json");
		break;
	case DiggerUtils::DiggerGameMode::Coop:
		SaveToJson(name, score, "Scores/HighScoreCoop.json");
		break;
	case DiggerUtils::DiggerGameMode::Versus:
		SaveToJson(name, score, "Scores/HighScoreVersus.json");
		break;
	}
}

void HighScoreComponent::SaveToJson(const std::string& name, int score, const std::string& filename)
{
	nlohmann::json jsonFile{ dae::ResourceManager::GetInstance().GetJsonFile(filename) };
	nlohmann::json json;
	json["name"] = name;
	json["score"] = score;
	auto now = std::chrono::system_clock::now();
	std::stringstream ss;
	const auto date = std::chrono::year_month_day(std::chrono::floor<std::chrono::days>(now));
	ss << date;
	json["date"] = ss.str();
	jsonFile["scores"].emplace_back(json);
	std::ofstream file(std::string(dae::ResourceManager::GetInstance().GetDataPath() + filename));
	if(!file.is_open())
	{
		std::cerr << "HighScoreComponent::SaveToJson: Failed to open file\n";
		return;
	}
	std::string str = jsonFile.dump(4);
	file << str;
	file.close();
}
