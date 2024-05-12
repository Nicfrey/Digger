#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"

#include <fstream>
#include <iostream>

#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

void dae::ResourceManager::Init(const std::string& dataPath)
{
	m_dataPath = dataPath;

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

std::shared_ptr<dae::Texture2D> dae::ResourceManager::LoadTexture(const std::string& file) const
{
	const auto fullPath = m_dataPath + file;
	auto texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}
	return std::make_shared<Texture2D>(texture);
}

std::shared_ptr<dae::Font> dae::ResourceManager::LoadFont(const std::string& file, unsigned int size) const
{
	return std::make_shared<Font>(m_dataPath + file, size);
}

nlohmann::json dae::ResourceManager::GetJsonFile(const std::string& filepath) const
{
	nlohmann::json json;
	std::ifstream file{ m_dataPath + filepath };
	if(file.is_open())
	{
		json = nlohmann::json::parse(file);
		file.close();
	}
	else
	{
		throw std::runtime_error("Failed to open as a json file " + m_dataPath + filepath);
	}
	return json;
}

std::string dae::ResourceManager::GetDataPath() const
{
	return m_dataPath;
}
