#include "LevelComponent.h"

#include <windows.h>
#include <iostream>
#include <Xinput.h>


#include "AnimatorComponent.h"
#include "BoxCollider2D.h"
#include "DiggerCommands.h"
#include "DiggerTransitionAnim.h"
#include "EmeraldComponent.h"
#include "GameObject.h"
#include "Graph.h"
#include "imgui.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "json.hpp"
#include "MoneyBagComponent.h"
#include "ResourceManager.h"
#include "SpriteComponent.h"
#include "HealthComponent.h"
#include "Observer.h"
#include "PlayerComponent.h"
#include "ScoreComponent.h"
#include "UIPlayerComponent.h"

LevelComponent::LevelComponent() : BaseComponent(nullptr), m_pGraph{new GraphUtils::Graph{}}
{
}

LevelComponent::~LevelComponent()
{
	delete m_pGraph;
	m_pGraph = nullptr;
}

LevelComponent::LevelComponent(const LevelComponent& other): BaseComponent(other),
                                                             m_pGraph{new GraphUtils::Graph{*other.m_pGraph}}
{
}

LevelComponent::LevelComponent(LevelComponent&& other) noexcept : BaseComponent{std::move(other)},
                                                                  m_pGraph{other.m_pGraph}
{
	other.m_pGraph = nullptr;
}

LevelComponent& LevelComponent::operator=(const LevelComponent& other)
{
	if (this == &other)
	{
		return *this;
	}
	BaseComponent::operator=(other);
	delete m_pGraph;
	m_pGraph = nullptr;
	m_ShortestPath.clear();
	m_pGraph = new GraphUtils::Graph{*other.m_pGraph};
	return *this;
}

LevelComponent& LevelComponent::operator=(LevelComponent&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}
	BaseComponent::operator=(std::move(other));
	delete m_pGraph;
	m_pGraph = nullptr;
	m_pGraph = other.m_pGraph;
	other.m_pGraph = nullptr;
	return *this;
}

std::shared_ptr<BaseComponent> LevelComponent::Clone() const
{
	return std::make_shared<LevelComponent>(*this);
}

void LevelComponent::Update()
{
	for(auto& player: m_Players)
	{
		auto node{ m_pGraph->GetClosestNode(player->GetWorldPosition()) };
		if(!node->CanBeVisited())
		{
			node->SetCanBeVisited(true);
		}
	}
}


void LevelComponent::Init()
{
	//EventManager::GetInstance().AddEvent("LoadLevel1", this, &LevelComponent::LoadLevel);
	EventManager::GetInstance().AddEvent("LoadLevel2",this,&LevelComponent::SecondLevel);
	EventManager::GetInstance().AddEvent("LoadLevel3", this, &LevelComponent::ThirdLevel);
}

void LevelComponent::RenderGUI()
{
	dae::GameObject* player{dae::SceneManager::GetInstance().GetGameObjectByTag("Player")};
	ImGui::Begin("Graph");
	for (size_t i{}; i < m_pGraph->GetNodes().size(); ++i)
	{
		const GraphUtils::GraphNode* node{m_pGraph->GetNode(static_cast<int>(i))};
		if (node->CanBeVisited())
			ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(node->GetPosition().x, node->GetPosition().y), 15,
			                                            IM_COL32(255, 0, 0, 255));
		else
			ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(node->GetPosition().x, node->GetPosition().y), 15,
			                                            IM_COL32(0, 255, 255, 255));
		for (const auto& neighbor : node->GetNeighbors())
		{
			const GraphUtils::GraphNode* neighborNode{neighbor.first};
			ImGui::GetWindowDrawList()->AddLine(ImVec2(node->GetPosition().x, node->GetPosition().y),
			                                    ImVec2(neighborNode->GetPosition().x, neighborNode->GetPosition().y),
			                                    IM_COL32(0, 255, 0, 255));
		}
	}
	for (size_t i{}; i < m_ShortestPath.size(); ++i)
	{
		const GraphUtils::GraphNode* node{m_ShortestPath[i]};
		ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(node->GetPosition().x, node->GetPosition().y), 15,
		                                            IM_COL32(0, 255, 0, 255));
	}
	if (player)
	{
		GraphUtils::GraphNode* closestNode{m_pGraph->GetClosestNode(player->GetWorldPosition())};
		ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(closestNode->GetPosition().x, closestNode->GetPosition().y),
		                                            15, IM_COL32(255, 255, 255, 255));
	}
	ImGui::End();
}

void LevelComponent::FirstLevel()
{
	LoadLevel(1);
}

void LevelComponent::SecondLevel()
{
	LoadLevel(2);
}

void LevelComponent::ThirdLevel()
{
	LoadLevel(3);
}

void LevelComponent::LoadLevel(int level)
{
	constexpr int maxRow{ 10 };
	constexpr int maxColumn{ 15 };

	// Read from json
	nlohmann::json json{ dae::ResourceManager::GetInstance().GetJsonFile("Levels/Level" + std::to_string(level) + ".json") };
	// Init background
	int numberLevel{ json["NumberLevel"].get<int>() };
	CreateBackgroundLevel(numberLevel);

	// Init graph
	for (int i{}; i < maxRow; ++i)
	{
		for (int j{}; j < maxColumn; ++j)
		{
			const int currentIndex{ i * maxColumn + j };
			GraphUtils::GraphNode* current{
				m_pGraph->AddNode(glm::vec3{
					m_StartPos.x + 35 * static_cast<float>(j), m_StartPos.y + 35 * static_cast<float>(i), 0
				})
			};
			current->SetCanBeVisited(false);
			if (j != 0)
			{
				GraphUtils::GraphNode* left{ m_pGraph->GetNode(currentIndex - 1) };
				left->AddNeighbor(current, glm::distance(current->GetPosition(), left->GetPosition()));
				current->AddNeighbor(left, glm::distance(current->GetPosition(), left->GetPosition()));
			}
			const int indexTop{ currentIndex - maxColumn };
			if (i > 0)
			{
				GraphUtils::GraphNode* top{ m_pGraph->GetNode(indexTop) };
				top->AddNeighbor(current, glm::distance(current->GetPosition(), top->GetPosition()));
				current->AddNeighbor(top, glm::distance(current->GetPosition(), top->GetPosition()));
			}
		}
	}


	// Init Players
	int indexPlayer{};
	for (auto data : json["SpawnPointPlayers"])
	{
		// TODO Check if we are selecting 2 players coop
		const glm::vec2 pos{ data.at("x"), data.at("y") };
		CreatePlayerAtIndex(GetIndexFromPosition(pos, maxColumn), indexPlayer);
		++indexPlayer;
	}

	// Init the spawn point
	auto spawnEnemy = json["SpawnPointEnemy"];
	m_SpawnPointEnemy = GetVectorFromJson(json["SpawnPointEnemy"]);
	// TODO Check if we are selecting 2 players versus

	// Set the graph index to be visited
	for (auto data : json["CanBeVisited"])
	{
		const glm::vec2 pos{ data.at("x"), data.at("y") };
		m_pGraph->GetNode(static_cast<int>(pos.y) * maxColumn + static_cast<int>(pos.x))->SetCanBeVisited(true);
	}

	for (auto data : json["Emerald"])
	{
		const glm::vec2 pos{ data.at("x"), data.at("y") };
		CreateEmeraldAtIndex(static_cast<int>(pos.y) * maxColumn + static_cast<int>(pos.x));
	}
}

void LevelComponent::SetToCoop()
{
	m_GameMode = GameMode::Coop;
}

void LevelComponent::SetToSinglePlayer()
{
	m_GameMode = GameMode::SinglePlayer;
}

void LevelComponent::SetToVersus()
{
	m_GameMode = GameMode::Versus;
}

glm::vec2 LevelComponent::GetVectorFromJson(const nlohmann::json& json)
{
	return glm::vec2{json.at("x"), json.at("y")};
}

void LevelComponent::CreateEmeraldAtIndex(int index)
{
	const auto pos = m_pGraph->GetNode(index)->GetPosition();
	const std::shared_ptr emerald{std::make_shared<dae::GameObject>()};
	const auto sprite = std::make_shared<SpriteComponent>("SpritesItems.png", 3, 3);
	emerald->AddComponent(std::make_shared<BoxCollider2D>(sprite->GetShape().width, sprite->GetShape().height));
	const auto item{std::make_shared<EmeraldComponent>()};
	emerald->AddComponent(sprite);
	emerald->AddComponent(item);
	emerald->SetLocalPosition(pos);
	dae::SceneManager::GetInstance().Instantiate(emerald);
}

void LevelComponent::CreateMoneyBagAtIndex(int index)
{
	const auto pos{m_pGraph->GetNode(index)->GetPosition()};
	const std::shared_ptr moneyBag{std::make_shared<dae::GameObject>()};
	const auto sprite{std::make_shared<SpriteComponent>("SpriteItems.png", 3, 3)};
	moneyBag->AddComponent(std::make_shared<BoxCollider2D>(sprite->GetShape().width, sprite->GetShape().height));
	const auto item{std::make_shared<MoneyBagComponent>()};
	const auto animator{std::make_shared<AnimatorComponent>()};
	Animation idle{.name = "Idle", .frames = {1}, .loop = true, .spriteComponent = sprite};
	Animation isFalling
	{
		.name = "IsFalling", . frames = {0, 1, 2}, .frameTime = 0.4f, .loop = true, .spriteComponent = sprite
	};
	Animation isDestroyed
	{
		.name = "IsDestroyed", . frames = {3, 4, 5}, .frameTime = 0.4f, .loop = false, .spriteComponent = sprite
	};
	Animation isDestroyedIdle
	{
		.name = "IsDestroyedIdle", . frames = {5}, .frameTime = 0.4f, .loop = true, .spriteComponent = sprite
	};
	TransitionMoneyBagIsIdle* transitionIdle{new TransitionMoneyBagIsIdle{}};
	TransitionMoneyBagCanFall* transitionCanFall{new TransitionMoneyBagCanFall{}};
	TransitionMoneyBagIsDestroyed* transitionDestroyed{new TransitionMoneyBagIsDestroyed{}};
	TransitionMoneyBagIsDestroyedIdle* transitionDestroyedIdle{new TransitionMoneyBagIsDestroyedIdle{}};
	animator->AddTransition(idle,isFalling,transitionCanFall);
	animator->AddTransition(isFalling, idle, transitionIdle);
	animator->AddTransition(idle, isDestroyed, transitionDestroyed);
	animator->AddTransition(isDestroyed, isDestroyedIdle, transitionDestroyedIdle);
	moneyBag->AddComponent(item);
	moneyBag->AddComponent(animator);
	moneyBag->AddComponent(sprite);
	moneyBag->SetLocalPosition(pos);
	dae::SceneManager::GetInstance().Instantiate(moneyBag);
}

void LevelComponent::CreateBackgroundLevel(int level)
{
	const std::shared_ptr background{ std::make_shared<dae::GameObject>() };
	const std::shared_ptr texture{ std::make_shared<TextureComponent>("Backgrounds/" + std::to_string(level) + ".png") };
	background->AddComponent(texture);
	dae::SceneManager::GetInstance().Instantiate(background);
}

void LevelComponent::CreatePlayerAtIndex(int index, int player)
{
	auto fontSmall = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	const auto pos{ m_pGraph->GetNode(index)->GetPosition() };
	auto go{ std::make_shared<dae::GameObject>() };
	const auto spritePlayer1{ std::make_shared<SpriteComponent>("SpritesPlayers.png",4,4) };
	auto healthComponent{ std::make_shared<HealthComponent>() };
	auto scoreComponent{ std::make_shared<ScoreComponent>() };
	auto uiComponent{ std::make_shared<UIPlayerComponent>(fontSmall) };
	auto boxCollider{ std::make_shared<BoxCollider2D>(spritePlayer1->GetShape().width,spritePlayer1->GetShape().height) };
	auto playerComponent{ std::make_shared<PlayerComponent>() };
	auto animator{ std::make_shared<AnimatorComponent>() };
	Animation idlePlayer{ .name = "Idle",.frames = {0,1,2},.frameTime = 0.1f, .spriteComponent = spritePlayer1 };
	if(player == 1)
	{
		idlePlayer.frames = { 8,9,10 };
	}
	Animation idleWithoutShoot{ .name = "IdleWithoutShoot",.frames{4,5,6},.frameTime = 0.1f,.spriteComponent = spritePlayer1 };
	if (player == 1)
	{
		idleWithoutShoot.frames = { 12,13,14 };
	}
	Animation deadAnim{ .name = "DeadAnim", .frames = {3},.spriteComponent = spritePlayer1 };
	if (player == 1)
	{
		idleWithoutShoot.frames = { 11 };
	}
	TransitionPlayerNoProjectile* transitionNoProjectile{ new TransitionPlayerNoProjectile() };
	TransitionPlayerHasProjectile* transitionProjectile{ new TransitionPlayerHasProjectile{} };
	TransitionPlayerIsDead* transitionDead{ new TransitionPlayerIsDead{} };
	animator->AddTransition(idlePlayer, idleWithoutShoot, transitionNoProjectile);
	animator->AddTransition(idleWithoutShoot, idlePlayer, transitionProjectile);
	animator->AddTransition(idlePlayer, deadAnim, transitionDead);
	animator->AddTransition(idleWithoutShoot, deadAnim, transitionDead);
	if (!animator->SetStartAnimation(idlePlayer))
	{
		std::cerr << "Failed to set start animation" << '\n';
	}
	uiComponent->SetPosition(0, 150);
	go->AddComponent(healthComponent);
	go->AddComponent(uiComponent);
	go->AddComponent(scoreComponent);
	go->AddComponent(spritePlayer1);
	go->AddComponent(boxCollider);
	go->AddComponent(playerComponent);
	go->AddComponent(animator);
	std::shared_ptr moveUpCommand{ std::make_shared<MoveCommand>(go.get(),glm::vec2{0,-1}) };
	std::shared_ptr moveDownCommand{ std::make_shared<MoveCommand>(go.get(),glm::vec2{0,1}) };
	std::shared_ptr moveLeftCommand{ std::make_shared<MoveCommand>(go.get(),glm::vec2{-1,0}) };
	std::shared_ptr moveRightCommand{ std::make_shared<MoveCommand>(go.get(),glm::vec2{1,0}) };
	std::shared_ptr shootCommand{ std::make_shared<ShootCommand>(go.get()) };

	GamepadController* gamepadController{ new GamepadController{} };
	gamepadController->BindAction(moveUpCommand, XINPUT_GAMEPAD_DPAD_UP);
	gamepadController->BindAction(moveDownCommand, XINPUT_GAMEPAD_DPAD_DOWN);
	gamepadController->BindAction(moveLeftCommand, XINPUT_GAMEPAD_DPAD_LEFT);
	gamepadController->BindAction(moveRightCommand, XINPUT_GAMEPAD_DPAD_RIGHT);
	gamepadController->BindAction(shootCommand, XINPUT_GAMEPAD_B, KeyPressed);
	dae::InputManager::GetInstance().AddController(gamepadController);
	go->SetLocalPosition(pos);
	dae::SceneManager::GetInstance().Instantiate(go);
	m_Players.emplace_back(go);
}

int LevelComponent::GetIndexFromPosition(const glm::vec2& pos, int maxColumn)
{
	return static_cast<int>(pos.y) * maxColumn + static_cast<int>(pos.x);
}
