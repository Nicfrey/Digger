#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "FpsTextComponent.h"
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "Scene.h"
#include "TextureComponent.h"
#include "Time.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	auto transform = std::make_shared<dae::Transform>();
	const auto tmBackground{std::make_shared<TextureComponent>()};
	tmBackground->SetTexture("background.tga");
	go->AddComponent(transform);
	go->AddComponent(tmBackground);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	transform = std::make_shared<dae::Transform>();
	const std::shared_ptr<TextureComponent> tmLogo{ std::make_shared<TextureComponent>()};
	tmLogo->SetTexture("logo.tga");
	transform->SetPosition(216, 180);
	go->AddComponent(tmLogo);
	go->AddComponent(transform);
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_shared<dae::GameObject>();
	transform = std::make_shared<dae::Transform>();
	const auto textManager{ std::make_shared<dae::TextComponent>("Programming 4 Assignment",font) };
	transform->SetPosition(80, 20);
	go->AddComponent(textManager);
	go->AddComponent(transform);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	const auto textManagerFPS{ std::make_shared<FpsTextComponent>(font) };
	transform = std::make_shared<dae::Transform>();
	transform->SetPosition(0, 20);
	go->AddComponent(transform);
	go->AddComponent(textManagerFPS);
	scene.Add(go);

	auto goParent = std::make_shared<dae::GameObject>();
	transform = std::make_shared<dae::Transform>();
	const auto textTestParent{ std::make_shared<dae::TextComponent>("TestParent",font) };
	transform->SetPosition(217, 60);
	goParent->AddComponent(transform);
	goParent->AddComponent(textTestParent);
	scene.Add(goParent);

	go = std::make_shared<dae::GameObject>();
	transform = std::make_shared<dae::Transform>();
	const auto textTestChildren{ std::make_shared<dae::TextComponent>("TestChildren",font) };
	transform->SetPosition(217, 80);
	go->AddComponent(transform);
	go->SetParent(goParent);
	go->AddComponent(textTestChildren);
	scene.Add(go);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}