#pragma once
#include <memory>

#include "UIElementComponent.h"
#include "Utils.h"

class ButtonComponent;

class KeyboardComponent final : public UIElementComponent
{
public:
	KeyboardComponent(const std::string& name, float startPosY);
	~KeyboardComponent() override = default;
	void RenderElement() const override;
	void DeletePreviousLetter();
	void AddLetter();
	void SelectTop();
	void SelectDown();
	void SelectLeft();
	void SelectRight();
	void OnClick(const glm::vec2 vec);

private:
	const std::string m_Alphabet[36] = { "A","B","C","D","E","F","G","H","I","J","K","L","M",
			"N","O","P","Q","R","S","T","U","V","W","X","Y","Z","0","1","2","3","4","5","6","7","8","9" };
	std::unique_ptr<ButtonComponent> m_ButtonsLetter[36];
	int m_CurrentSelected{};
	int m_PreviousSelected{};
	std::string m_CurrentWord{};
};

