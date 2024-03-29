#include <stdexcept>
#define WIN32_LEAN_AND_MEAN
#define MS_PER_FRAME 8
#define FIXED_TIME_STEP 0.25f

#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"

#include <steam_api_common.h>
#include <thread>

#include "Achievement.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Time.h"

SDL_Window* g_window{};

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
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
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
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	Time::UpdateLastTime();
	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	Achievement_t g_Achievements[] =
	{
		_ACH_ID(ACH_WIN_ONE_GAME, "Vainqueur"),
		_ACH_ID(ACH_WIN_100_GAMES, "Champion"),
		_ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellaire"),
		_ACH_ID(ACH_TRAVEL_FAR_SINGLE, "�toile"),
	};

	auto achievement = new Achievement(g_Achievements,4);
	sceneManager.Init();
	float lag{0.f};

	// todo: this update loop could use some work.
	bool doContinue = true;
	while (doContinue)
	{
		SteamAPI_RunCallbacks();
		Time::Update();
		lag += Time::GetDeltaTime();

		doContinue = input.ProcessInput();

		while(lag >= FIXED_TIME_STEP)
		{
			sceneManager.FixedUpdate();
			lag -= FIXED_TIME_STEP;
		}
		sceneManager.Update();
		renderer.Render();

		const auto sleepTime{ Time::GetCurrent() + std::chrono::milliseconds(MS_PER_FRAME) - std::chrono::high_resolution_clock::now() };
		std::this_thread::sleep_for(sleepTime);
	}
	delete achievement;
}
