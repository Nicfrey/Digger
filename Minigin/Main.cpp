#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "FpsCounterComponent.h"
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextManager.h"
#include "Scene.h"
#include "TextureManager.h"
#include "Time.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	const auto tmBackground{std::make_shared<TextureManager>()};
	tmBackground->SetTexture("background.tga");
	go->AddComponent(tmBackground);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	const std::shared_ptr<TextureManager> tmLogo{ std::make_shared<TextureManager>()};
	tmLogo->SetTexture("logo.tga");
	go->SetPosition(216, 180);
	go->AddComponent(tmLogo);
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_shared<dae::GameObject>();
	const auto textManager{ std::make_shared<dae::TextManager>("Programming 4 Assignment",font) };
	go->SetPosition(80, 20);
	go->AddComponent(textManager);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	const auto textManagerFPS{ std::make_shared<FpsCounterComponent>(font) };
	go->SetPosition(0, 20);
	go->AddComponent(textManagerFPS);
	scene.Add(go);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}