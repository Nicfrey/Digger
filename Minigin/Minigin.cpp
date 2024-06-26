#include "Minigin.h"

#include <stdexcept>
#define WIN32_LEAN_AND_MEAN
#define MS_PER_FRAME 8
#define FIXED_TIME_STEP 0.25f

#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <thread>

#include "GameState.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TimeEngine.h"
#include "Utils.h"
#include "WidgetManager.h"

SDL_Window* g_window{};
const glm::vec2 dae::Minigin::m_Window{ 640, 480 };

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string &dataPath)
{
	srand(static_cast<unsigned int>(time(nullptr)));
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_OGG) < 0) 
	{
		throw std::runtime_error(std::string("SDL mixer error: ") + Mix_GetError());
	}

	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048) < 0)
	{
		throw std::runtime_error(std::string("SDL mixer error: ") + Mix_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		static_cast<int>(m_Window.x),
		static_cast<int>(m_Window.y),
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	Mix_CloseAudio();
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	auto lastTime{ std::chrono::high_resolution_clock::now() };

	auto& gameState = GameState::GetInstance();
	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& timerManager = TimerManager::GetInstance();
	sceneManager.Init();
	float lag{0.f};

	// todo: this update loop could use some work.
	bool doContinue = true;
	while (doContinue)
	{
		const auto currentTime{ std::chrono::high_resolution_clock::now() };
		TimeEngine::GetInstance().SetDeltaTime(std::chrono::duration<float>(currentTime - lastTime).count());
		lastTime = currentTime;
		lag += TimeEngine::GetInstance().GetDeltaTime();

		doContinue = input.ProcessInput();

		while(lag >= FIXED_TIME_STEP)
		{
			sceneManager.FixedUpdate();
			lag -= FIXED_TIME_STEP;
		}
		sceneManager.OnCollisionUpdate();
		gameState.Update();
		timerManager.Update();
		sceneManager.Update();
		renderer.Render();

		sceneManager.OnDestroy();
		const auto sleepTime{ currentTime + std::chrono::milliseconds(MS_PER_FRAME) - std::chrono::high_resolution_clock::now() };
		std::this_thread::sleep_for(sleepTime);
	}
}
