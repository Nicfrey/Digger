#include "LevelComponent.h"

#include <windows.h>
#include <iostream>
#include <Xinput.h>

#include "Scene.h"
#include "AnimatorComponent.h"
#include "BackgroundComponent.h"
#include "BoxCollider2D.h"
#include "DiggerCommands.h"
#include "DiggerTransitionAnim.h"
#include "DiggerUtils.h"
#include "EmeraldComponent.h"
#include "EnemyComponent.h"
#include "EnemySpawnerComponent.h"
#include "GameInstance.h"
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
#include "Minigin.h"
#include "NavMeshAgentComponent.h"
#include "Observer.h"
#include "PlayerComponent.h"
#include "Renderer.h"
#include "ScoreComponent.h"
#include "ThreadPool.h"
#include "UIPlayerComponent.h"

LevelComponent::LevelComponent() : BaseComponent(nullptr), m_pGraph{new GraphUtils::Graph{}},
                                   m_pThreadPool{std::make_unique<ThreadPool>(std::thread::hardware_concurrency())}
{
	EventManager::GetInstance().AddEvent("LoadLevel", this, &LevelComponent::LoadLevel);
	EventManager::GetInstance().AddEvent("PlayerMoving", this, &LevelComponent::UpdateGraphPlayers);
	EventManager::GetInstance().AddEvent("EnemyMoving", this, &LevelComponent::UpdateGraphEnemies);
	EventManager::GetInstance().AddEvent("ReloadCurrentLevel", this, &LevelComponent::RespawnPlayers);
	EventManager::GetInstance().AddEvent("EmeraldCollected", this, &LevelComponent::CheckRemainingEmeralds);
}

LevelComponent::~LevelComponent()
{
	delete m_pGraph;
	m_pGraph = nullptr;
}

GraphUtils::Graph* LevelComponent::GetGraph() const
{
	return m_pGraph;
}

bool LevelComponent::IsNodeMoneyBag(const GraphUtils::GraphNode* node) const
{
	const auto moneyBags{dae::SceneManager::GetInstance().GetGameObjectsWithComponent<MoneyBagComponent>()};
	for (auto& moneyBag : moneyBags)
	{
		auto state = moneyBag->GetComponent<MoneyBagComponent>()->GetState();
		if (state == MoneyBagComponent::StateMoneyBag::CanFall || state == MoneyBagComponent::StateMoneyBag::Idle)
		{
			if (m_pGraph->GetClosestNode(moneyBag->GetWorldPosition()) == node)
			{
				return true;
			}
		}
	}
	return false;
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


void LevelComponent::RenderGUI()
{
	const std::shared_ptr player{dae::SceneManager::GetInstance().GetGameObjectByTag("Player")};
	ImGui::Begin("LevelComponent");
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
			if (!node->GetTransitionCanBeVisited(neighbor.first))
			{
				ImGui::GetWindowDrawList()->AddLine(ImVec2(node->GetPosition().x, node->GetPosition().y),
				                                    ImVec2(neighbor.first->GetPosition().x,
				                                           neighbor.first->GetPosition().y),
				                                    IM_COL32(255, 0, 0, 255));
			}
			else
			{
				const GraphUtils::GraphNode* neighborNode{ neighbor.first };
				ImGui::GetWindowDrawList()->AddLine(ImVec2(node->GetPosition().x, node->GetPosition().y),
					ImVec2(neighborNode->GetPosition().x, neighborNode->GetPosition().y),
					IM_COL32(0, 255, 0, 255));
			}
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

void LevelComponent::OnDestroy()
{
	EventManager::GetInstance().RemoveEvent("LoadLevel", this, &LevelComponent::LoadLevel);
	EventManager::GetInstance().RemoveEvent("PlayerMoving", this, &LevelComponent::UpdateGraphPlayers);
	EventManager::GetInstance().RemoveEvent("ReloadCurrentLevel", this, &LevelComponent::RespawnPlayers);
	EventManager::GetInstance().RemoveEvent("EnemyMoving", this, &LevelComponent::UpdateGraphEnemies);
}

void LevelComponent::FreeSpaceMoneyBag(GraphUtils::GraphNode* node) const
{
	node->SetCanBeVisited(true);
	node->SetTransitionCanBeVisited(node->GetBottomNeighbor());
	node->GetBottomNeighbor()->SetTransitionCanBeVisited(node);
	// Apply thread here
	m_pThreadPool->EnqueueTask([this, node]
	{
		const auto backgrounds{
			dae::SceneManager::GetInstance().GetGameObjectsWithComponent<BackgroundComponent>()
		};
		for (const auto background : backgrounds)
		{
			const auto currentNode{background->GetWorldPosition()};
			const auto closestNode{GetGraph()->GetClosestNode(currentNode)};
			if ((closestNode == node || closestNode == node->GetBottomNeighbor()) && glm::distance(
				closestNode->GetPosition(), background->GetWorldPosition()) < 23.f)
			{
				background->Destroy();
			}
		}
	});
}

void LevelComponent::CreateSpawnerEnemy(int index) const
{
	const glm::vec3 pos{m_pGraph->GetNode(index)->GetPosition()};
	const auto go{std::make_shared<dae::GameObject>()};
	go->SetLocalPosition(pos);
	const auto spawnerEnemy{std::make_shared<EnemySpawnerComponent>()};
	if (m_GameMode == DiggerUtils::DiggerGameMode::Versus)
	{
		spawnerEnemy->CanSpawnPlayer();
	}
	go->AddComponent(spawnerEnemy);
	dae::SceneManager::GetInstance().Instantiate(go);
}

void LevelComponent::LoadLevel()
{
	GameInstance::GetInstance().GetValue("CurrentLevel", m_Level);
	GameInstance::GetInstance().GetValue("CurrentGameMode", m_GameMode);

	nlohmann::json json{
		dae::ResourceManager::GetInstance().GetJsonFile("Levels/Level" + std::to_string(m_Level) + ".json")
	};
	InitializeLevel(json);
	// Init Players
	int indexPlayer{};
	for (auto data : json["SpawnPointPlayers"])
	{
		if (m_GameMode != DiggerUtils::DiggerGameMode::Coop && indexPlayer == 1)
		{
			break;
		}
		const glm::vec2 pos{data.at("x"), data.at("y")};
		CreatePlayerAtIndex(GetIndexFromPosition(pos, m_MaxColumn), indexPlayer);
		++indexPlayer;
	}
}

void LevelComponent::InitializeLevel(const nlohmann::json& json)
{
	// Init graph
	InitializeGraph(json);

	// Init background
	const int numberLevel{json["NumberLevel"].get<int>()};
	CreateBackgroundLevel(numberLevel);

	// Init the spawn point
	const auto& spawnEnemy = json["SpawnPointEnemy"];
	CreateSpawnerEnemy(GetIndexFromPosition(GetVectorFromJson(spawnEnemy), m_MaxColumn));


	for (auto data : json["Emerald"])
	{
		const glm::vec2 pos{data.at("x"), data.at("y")};
		CreateEmeraldAtIndex(static_cast<int>(pos.y) * m_MaxColumn + static_cast<int>(pos.x));
	}

	for (auto data : json["Moneybag"])
	{
		const glm::vec2 pos{data.at("x"), data.at("y")};
		CreateMoneyBagAtIndex(static_cast<int>(pos.y) * m_MaxColumn + static_cast<int>(pos.x));
	}
	CreateUIObject();
	EventManager::GetInstance().NotifyEvent("LevelLoaded");
}

void LevelComponent::RespawnPlayers()
{
	// Delete all the enemies and the spawner
	const auto enemies{dae::SceneManager::GetInstance().GetGameObjectsWithComponent<EnemyComponent>()};
	for (const auto& enemy : enemies)
	{
		enemy->Destroy();
	}
	if (const auto spawner{dae::SceneManager::GetInstance().GetGameObjectWithComponent<EnemySpawnerComponent>()})
	{
		spawner->Destroy();
	}

	auto json{dae::ResourceManager::GetInstance().GetJsonFile("Levels/Level" + std::to_string(m_Level) + ".json")};

	// Init the spawn point
	const auto spawnEnemy = json["SpawnPointEnemy"];
	CreateSpawnerEnemy(GetIndexFromPosition(GetVectorFromJson(spawnEnemy), m_MaxColumn));

	const auto players{dae::SceneManager::GetInstance().GetGameObjectsWithComponent<PlayerComponent>()};
	int indexPlayer{};
	for (auto data : json["SpawnPointPlayers"])
	{
		if (m_GameMode != DiggerUtils::DiggerGameMode::Coop && indexPlayer == 1)
		{
			break;
		}
		const glm::vec2 pos{data.at("x"), data.at("y")};
		players[indexPlayer]->
			SetLocalPosition(m_pGraph->GetNode(GetIndexFromPosition(pos, m_MaxColumn))->GetPosition());
		players[indexPlayer]->GetComponent<HealthComponent>()->SetAlive();
		players[indexPlayer]->GetComponent<PlayerComponent>()->HandleRespawn();
		++indexPlayer;
	}
	EventManager::GetInstance().NotifyEvent("LevelLoaded");
}

glm::vec2 LevelComponent::GetVectorFromJson(const nlohmann::json& json)
{
	return glm::vec2{json.at("x"), json.at("y")};
}

void LevelComponent::CreateEmeraldAtIndex(int index)
{
	const auto pos = m_pGraph->GetNode(index)->GetPosition();
	const std::shared_ptr emerald{std::make_shared<dae::GameObject>()};
	const auto sprite = std::make_shared<SpriteComponent>("Emerald", "SpritesItems.png", 3, 3);
	const auto boxCollider = std::make_shared<BoxCollider2D>(sprite->GetShape().width, sprite->GetShape().height);
	boxCollider->SetIsStatic(true);
	emerald->AddComponent(boxCollider);
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
	const auto sprite{std::make_shared<SpriteComponent>("MoneyBag", "SpritesItems.png", 3, 3)};
	moneyBag->AddComponent(std::make_shared<BoxCollider2D>(sprite->GetShape().width - 5, sprite->GetShape().height));
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
	TransitionMoneyBagCanFall* transitionCanFall{new TransitionMoneyBagCanFall{}};
	TransitionMoneyBagIsDestroyed* transitionDestroyed{new TransitionMoneyBagIsDestroyed{}};
	TransitionMoneyBagIsDestroyedIdle* transitionDestroyedIdle{new TransitionMoneyBagIsDestroyedIdle{}};
	TransitionMoneyBagIsFalling* transitionIsFalling{new TransitionMoneyBagIsFalling{}};
	animator->AddTransition(idle, isFalling, transitionCanFall);
	animator->AddTransition(isFalling, idle, transitionIsFalling);
	animator->AddTransition(idle, isDestroyed, transitionDestroyed);
	animator->AddTransition(isDestroyed, isDestroyedIdle, transitionDestroyedIdle);
	if (!animator->SetStartAnimation(idle))
	{
		std::cerr << "Failed to set start animation MoneyBag" << '\n';
	}
	moneyBag->AddComponent(item);
	moneyBag->AddComponent(animator);
	moneyBag->AddComponent(sprite);
	moneyBag->SetLocalPosition(pos);
	dae::SceneManager::GetInstance().Instantiate(moneyBag);
}

void LevelComponent::CreateBackgroundLevel(int level)
{
	const auto windowSize{dae::Minigin::m_Window};
	glm::vec2 currentSize{0, 0};
	const std::shared_ptr sprite{
		std::make_shared<SpriteComponent>("Background", "Backgrounds/back" + std::to_string(level) + ".png")
	};
	while (currentSize.y <= windowSize.y)
	{
		while (currentSize.x <= windowSize.x)
		{
			const GraphUtils::GraphNode* current{m_pGraph->GetClosestNode(glm::vec3{currentSize, 0})};
			if (current->CanBeVisited() && glm::distance(current->GetPosition(), glm::vec3{currentSize, 0}) < 20.f)
			{
				currentSize.x += sprite->GetShape().width;
				continue;
			}
			const std::shared_ptr spriteComponent{
				std::make_shared<SpriteComponent>("Background", "Backgrounds/back" + std::to_string(level) + ".png")
			};
			const std::shared_ptr background{std::make_shared<dae::GameObject>()};
			const std::shared_ptr component{std::make_shared<BackgroundComponent>()};
			const std::shared_ptr boxCollider{
				std::make_shared<BoxCollider2D>(sprite->GetShape().width, sprite->GetShape().height)
			};
			boxCollider->SetIsStatic(true);
			background->AddComponent(spriteComponent);
			background->AddComponent(component);
			background->AddComponent(boxCollider);
			background->SetLocalPosition(currentSize.x, currentSize.y);
			dae::SceneManager::GetInstance().Instantiate(background);
			currentSize.x += sprite->GetShape().width;
		}
		currentSize.x = 0;
		currentSize.y += sprite->GetShape().height;
	}
}

void LevelComponent::CreatePlayerAtIndex(int index, int player)
{
	auto fontSmall = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	const auto pos{m_pGraph->GetNode(index)->GetPosition()};
	auto go{std::make_shared<dae::GameObject>()};
	const auto spritePlayer1{
		std::make_shared<SpriteComponent>("PlayerSprite" + std::to_string(player), "SpritesPlayers.png", 4, 4)
	};
	auto healthComponent{std::make_shared<HealthComponent>()};
	auto scoreComponent{std::make_shared<ScoreComponent>()};
	auto boxCollider{
		std::make_shared<BoxCollider2D>(spritePlayer1->GetShape().width, spritePlayer1->GetShape().height - 5)
	};
	auto playerComponent{std::make_shared<PlayerComponent>()};
	auto navMeshAgentComponent{std::make_shared<NavMeshAgentComponent>(m_pGraph, 80.f, true)};
	auto animator{std::make_shared<AnimatorComponent>()};
	Animation idlePlayer{.name = "Idle", .frames = {0, 1, 2}, .frameTime = 0.1f, .spriteComponent = spritePlayer1};
	if (player == 1)
	{
		idlePlayer.frames = {8, 9, 10};
	}
	Animation idleWithoutShoot{
		.name = "IdleWithoutShoot", .frames{4, 5, 6}, .frameTime = 0.1f, .spriteComponent = spritePlayer1
	};
	if (player == 1)
	{
		idleWithoutShoot.frames = {12, 13, 14};
	}
	Animation deadAnim{.name = "DeadAnim", .frames = {3}, .spriteComponent = spritePlayer1};
	if (player == 1)
	{
		idleWithoutShoot.frames = {11};
	}
	TransitionPlayerNoProjectile* transitionNoProjectile{new TransitionPlayerNoProjectile()};
	TransitionPlayerHasProjectile* transitionProjectile{new TransitionPlayerHasProjectile{}};
	TransitionPlayerIsDead* transitionDead{new TransitionPlayerIsDead{}};
	TransitionPlayerIsAlive* transitionPlayerAlive{new TransitionPlayerIsAlive{}};
	animator->AddTransition(idlePlayer, idleWithoutShoot, transitionNoProjectile);
	animator->AddTransition(idleWithoutShoot, idlePlayer, transitionProjectile);
	animator->AddTransition(idlePlayer, deadAnim, transitionDead);
	animator->AddTransition(idleWithoutShoot, deadAnim, transitionDead);
	animator->AddTransition(deadAnim, idlePlayer, transitionPlayerAlive);
	if (!animator->SetStartAnimation(idlePlayer))
	{
		std::cerr << "Failed to set start animation" << '\n';
	}
	go->AddComponent(healthComponent);
	go->AddComponent(scoreComponent);
	go->AddComponent(spritePlayer1);
	go->AddComponent(boxCollider);
	go->AddComponent(playerComponent);
	go->AddComponent(animator);
	go->AddComponent(navMeshAgentComponent);
	std::shared_ptr moveUpCommand{std::make_shared<MovePlayerCommand>(go.get(), glm::vec2{0, -1})};
	std::shared_ptr moveDownCommand{std::make_shared<MovePlayerCommand>(go.get(), glm::vec2{0, 1})};
	std::shared_ptr moveLeftCommand{std::make_shared<MovePlayerCommand>(go.get(), glm::vec2{-1, 0})};
	std::shared_ptr moveRightCommand{std::make_shared<MovePlayerCommand>(go.get(), glm::vec2{1, 0})};
	std::shared_ptr shootCommand{std::make_shared<ShootCommand>(go.get())};

	GamepadController* gamepadController{new GamepadController{player}};
	gamepadController->BindAction(moveUpCommand, XINPUT_GAMEPAD_DPAD_UP);
	gamepadController->BindAction(moveDownCommand, XINPUT_GAMEPAD_DPAD_DOWN);
	gamepadController->BindAction(moveLeftCommand, XINPUT_GAMEPAD_DPAD_LEFT);
	gamepadController->BindAction(moveRightCommand, XINPUT_GAMEPAD_DPAD_RIGHT);
	gamepadController->BindAction(shootCommand, XINPUT_GAMEPAD_B, KeyPressed);
	dae::InputManager::GetInstance().AddController(gamepadController);
	if (player == 0)
	{
		dae::InputManager::GetInstance().BindCommand(moveUpCommand, SDL_SCANCODE_UP);
		dae::InputManager::GetInstance().BindCommand(moveDownCommand, SDL_SCANCODE_DOWN);
		dae::InputManager::GetInstance().BindCommand(moveLeftCommand, SDL_SCANCODE_LEFT);
		dae::InputManager::GetInstance().BindCommand(moveRightCommand, SDL_SCANCODE_RIGHT);
		dae::InputManager::GetInstance().BindCommand(moveUpCommand, SDL_SCANCODE_UP);
		dae::InputManager::GetInstance().BindCommand(shootCommand, SDL_SCANCODE_SPACE, KeyPressed);
	}
	go->SetLocalPosition(pos);
	dae::SceneManager::GetInstance().Instantiate(go);
}

int LevelComponent::GetIndexFromPosition(const glm::vec2& pos, int maxColumn)
{
	return static_cast<int>(pos.y) * maxColumn + static_cast<int>(pos.x);
}

void LevelComponent::UpdateGraphPlayers()
{
	const auto players{dae::SceneManager::GetInstance().GetGameObjectsWithComponent<PlayerComponent>()};
	for (size_t i{}; i < players.size(); ++i)
	{
		HandleFreeSpace(players[i]->GetComponent<PlayerComponent>()->GetPreviousNode(), players[i]);
	}
}

void LevelComponent::UpdateGraphEnemies()
{
	const auto enemies{dae::SceneManager::GetInstance().GetGameObjectsWithComponent<EnemyComponent>()};
	for (size_t i{}; i < enemies.size(); ++i)
	{
		auto enemyComp{enemies[i]->GetComponent<EnemyComponent>()};
		if (enemyComp->GetType() == EnemyComponent::EnemyType::Nobbins)
		{
			HandleFreeSpace(enemyComp->GetPreviousNode(), enemies[i]);
		}
	}
}

void LevelComponent::InitializeGraph(const nlohmann::json& json) const
{
	for (int i{}; i < m_MaxRow; ++i)
	{
		for (int j{}; j < m_MaxColumn; ++j)
		{
			const int currentIndex{i * m_MaxColumn + j};
			GraphUtils::GraphNode* current{
				m_pGraph->AddNode(glm::vec3{
					m_StartPos.x + 35 * static_cast<float>(j), m_StartPos.y + 35 * static_cast<float>(i), 0
				})
			};
			current->SetCanBeVisited(false);
			if (j != 0)
			{
				GraphUtils::GraphNode* left{m_pGraph->GetNode(currentIndex - 1)};
				left->AddNeighbor(current, glm::distance(current->GetPosition(), left->GetPosition()));
				current->AddNeighbor(left, glm::distance(current->GetPosition(), left->GetPosition()));
			}
			const int indexTop{currentIndex - m_MaxColumn};
			if (i > 0)
			{
				GraphUtils::GraphNode* top{m_pGraph->GetNode(indexTop)};
				top->AddNeighbor(current, glm::distance(current->GetPosition(), top->GetPosition()));
				current->AddNeighbor(top, glm::distance(current->GetPosition(), top->GetPosition()));
			}
		}
	}

	GraphUtils::GraphNode* currentNode{nullptr};
	GraphUtils::GraphNode* previousNode{nullptr};
	for (auto data : json["CanBeVisited"])
	{
		const glm::vec2 pos{data.at("x"), data.at("y")};
		currentNode = m_pGraph->GetNode(static_cast<int>(pos.y) * m_MaxColumn + static_cast<int>(pos.x));
		currentNode->SetCanBeVisited(true);
		if (previousNode != nullptr && currentNode->IsNodeNeighbor(previousNode))
		{
			previousNode->SetTransitionCanBeVisited(currentNode);
			currentNode->SetTransitionCanBeVisited(previousNode);
		}
		previousNode = currentNode;
	}
}

void LevelComponent::CheckRemainingEmeralds()
{
	const auto emeralds{dae::SceneManager::GetInstance().GetGameObjectsWithComponent<EmeraldComponent>()};
	std::cout << "Emeralds size: " << emeralds.size() << '\n';
	if (emeralds.size() == 1)
	{
		EventManager::GetInstance().NotifyEvent("PlayerWon");
	}
}

void LevelComponent::CheckRemainingEnemies()
{
	const auto enemies{dae::SceneManager::GetInstance().GetGameObjectsWithComponent<EnemyComponent>()};
	const auto spawner{dae::SceneManager::GetInstance().GetGameObjectWithComponent<EnemySpawnerComponent>()};
	if (enemies.empty() && spawner == nullptr)
	{
		EventManager::GetInstance().NotifyEvent("PlayerWon");
	}
}

void LevelComponent::CreateUIObject()
{
	const auto go{std::make_shared<dae::GameObject>()};
	const auto uiComp{std::make_shared<UIPlayerComponent>()};
	go->AddComponent(uiComp);
	dae::SceneManager::GetInstance().Instantiate(go);
}

void LevelComponent::HandleFreeSpace(GraphUtils::GraphNode*& previousNode,
                                     const std::shared_ptr<dae::GameObject>& object)
{
	auto currentNode = m_pGraph->GetClosestNode(object->GetWorldPosition());
	if (previousNode == nullptr)
	{
		std::cout << "previous node is nullptr\n";
		previousNode = currentNode;
	}
	if (previousNode != currentNode)
	{
		std::cout << "Update graph transition\n";
		m_pGraph->GetNode(previousNode)->SetTransitionCanBeVisited(currentNode);
		m_pGraph->GetNode(currentNode)->SetTransitionCanBeVisited(previousNode);
		previousNode = currentNode;
	}
	if (!currentNode->CanBeVisited())
	{
		currentNode->SetCanBeVisited(true);
		// Apply thread here
		m_pThreadPool->EnqueueTask([this]
		{
			std::cout << "REMOVE BACKGROUNDS\n";
			const auto backgrounds{
				dae::SceneManager::GetInstance().GetGameObjectsWithComponent<BackgroundComponent>()
			};
			for (const auto& background : backgrounds)
			{
				const auto node{background->GetWorldPosition()};
				const auto closestNode{GetGraph()->GetClosestNode(node)};
				if (closestNode->CanBeVisited() && glm::distance(closestNode->GetPosition(),
				                                                 background->GetWorldPosition()) < 10.f)
				{
					background->Destroy();
				}
			}
		});
	}
}
