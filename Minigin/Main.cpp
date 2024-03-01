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
#include "RotateComponent.h"
#include "TextComponent.h"
#include "Scene.h"
#include "TextureComponent.h"
#include "Time.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	const auto tmBackground{std::make_shared<TextureComponent>()};
	tmBackground->SetTexture("background.tga");
	go->AddComponent(tmBackground);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>(glm::vec3{216,180,0.f});
	const std::shared_ptr<TextureComponent> tmLogo{ std::make_shared<TextureComponent>()};
	tmLogo->SetTexture("logo.tga");
	go->AddComponent(tmLogo);
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_shared<dae::GameObject>();
	const auto textManager{ std::make_shared<dae::TextComponent>("Programming 4 Assignment",font) };
	go->SetLocalPosition(80, 20);
	go->AddComponent(textManager);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	const auto textManagerFPS{ std::make_shared<FpsTextComponent>(font) };
	go->SetLocalPosition(0, 20);
	go->AddComponent(textManagerFPS);
	scene.Add(go);

	const auto goParent = std::make_shared<dae::GameObject>();
	const auto characterText{ std::make_shared<TextureComponent>() };
	characterText->SetTexture("MainCharacter.png");
	auto rotation{ std::make_shared<RotateComponent>(glm::vec3{217,190,0.f}) };
	goParent->SetLocalPosition(217,200);
	goParent->AddComponent(characterText);
	goParent->AddComponent(rotation);
	scene.Add(goParent);

	go = std::make_shared<dae::GameObject>();
	const auto secondCharacterText{ std::make_shared<TextureComponent>() };
	secondCharacterText->SetTexture("SecondCharacter.png");
	rotation = std::make_shared<RotateComponent>(goParent->GetLocalPosition());
	go->SetLocalPosition(0, 20);
	go->AddComponent(secondCharacterText);
	go->AddComponent(rotation);
	go->SetParent(goParent, false);
	scene.Add(go);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}