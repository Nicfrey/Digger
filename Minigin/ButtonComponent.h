#pragma once
#include "BaseComponent.h"
#include "UIElementComponent.h"
#include "Utils.h"

class TextureComponent;

namespace dae
{
	class Font;
	class TextComponent;
}

class ButtonComponent : public UIElementComponent
{
public:
	ButtonComponent(const std::string& name, const glm::vec3& pos, const std::string& textButton, const std::shared_ptr<dae::Font>& font, const std::string& fileNameTexture = "");
	ButtonComponent(const std::string& name,const glm::vec3& pos,const std::shared_ptr<dae::TextComponent>& text, const std::shared_ptr<TextureComponent>& image = nullptr);
	template<typename ClassType>
	void SetOnButtonClick(ClassType* obj, void (ClassType::* funcPtr)());
	void SetOnButtonClick(const DelegateFnc& func);
	// For mouse only
	void OnClick(const glm::vec2& posMouse) const;
	// For controller only
	void OnPressed() const;
	void RenderElement() const override;
	void SetTextButton(const std::string& text);
	void SetPositionOffset(float x, float y);
	void SetPositionOffset(const glm::vec2& pos);
	Rectf GetBox() const;
private:
	Rectf m_Box;
	std::shared_ptr<dae::TextComponent> m_Text;
	std::shared_ptr<TextureComponent> m_Image;
	DelegateFnc m_OnButtonClickFunc;
	bool m_IsSelected;
};

template <typename ClassType>
void ButtonComponent::SetOnButtonClick(ClassType* obj, void(ClassType::* funcPtr)())
{
	m_OnButtonClickFunc = [obj, funcPtr]()
		{
			(obj->*funcPtr)();
		};
}

