#include <stdexcept>
#include "Renderer.h"

#include <iostream>
#include <SDL_opengl.h>

#include "imgui.h"
#include "implot.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "Utils.h"
#include "WidgetManager.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	WidgetManager::GetInstance().Update();
	glPushMatrix();
	glScalef(1.15f, 1.15f, 1.15f);
	SceneManager::GetInstance().Render();
	glPopMatrix();
	WidgetManager::GetInstance().Render();

	// Display Demo ImGui
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	SceneManager::GetInstance().RenderGUI();
	ImGui::ShowDemoWindow();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	ImPlot::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const Rectf& dstRect, const Rectf& srcRect) const
{
	const SDL_Rect srcRectSDL{ static_cast<int>(srcRect.topLeft.x), static_cast<int>(srcRect.topLeft.y),static_cast<int>(srcRect.width),static_cast<int>(srcRect.height)};
	const SDL_Rect destRectSDL{ static_cast<int>(dstRect.topLeft.x), static_cast<int>(dstRect.topLeft.y),static_cast<int>(dstRect.width),static_cast<int>(dstRect.height) };
	if(SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &srcRectSDL, &destRectSDL) != 0)
	{
		const auto error = SDL_GetError();
		std::cout << error << "\n";
	}
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }

SDL_Window* dae::Renderer::GetWindow() const
{
	return m_window;
}
