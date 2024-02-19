#pragma once
#include <memory>
#include "Transform.h"

class TextureManager;

namespace dae
{
	class Texture2D;

	// todo: this should become final.
	class GameObject 
	{
	public:
		virtual void Init();
		virtual void Update();
		virtual void Render() const;

		std::shared_ptr<TextureManager> GetComponent() const;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_transform{};
		// todo: mmm, every gameobject has a texture? Is that correct?
		std::shared_ptr<TextureManager> m_TextureManager{};
	};
}
