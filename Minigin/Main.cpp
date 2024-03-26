#include <steam_api.h>
#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <iostream>
#include <windows.h>
#include <Xinput.h>
#include "FpsTextComponent.h"
#include "GameObjectCommand.h"
#include "HealthComponent.h"
#include "InputManager.h"
#include "Minigin.h"
#include "MovementComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "Scene.h"
#include "SpriteComponent.h"
#include "TextureComponent.h"
#include "TrashTheCacheGOComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	const auto tmBackground{std::make_shared<TextureComponent>()};
	tmBackground->SetTexture("background.tga");
	go->AddComponent(tmBackground);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	const auto spritePlayer{ std::make_shared<SpriteComponent>("SpritesPlayers.png",4,4) };
	go->AddComponent(spritePlayer);
	scene.Add(go);
}

int main(int, char*[]) {
	if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
		return 1;
	}
	std::cout << "Successfully initialized steam." << std::endl;
	dae::Minigin engine("../Data/");
	engine.Run(load);
	SteamAPI_Shutdown();
    return 0;
}