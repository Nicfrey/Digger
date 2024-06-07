#include "KeyboardComponent.h"

#include <iostream>

#include "ButtonComponent.h"
#include "Minigin.h"
#include "ResourceManager.h"

KeyboardComponent::KeyboardComponent(const std::string& name, float startPosY): UIElementComponent(name)
{
	const glm::vec2 window = dae::Minigin::m_Window;
	const float width{window.x / 6.f};
	glm::vec3 startPos{40.f, startPosY, 0.f};
	const auto font{dae::ResourceManager::GetInstance().LoadFont("DiggerFont.ttf", 30)};
	float height{};
	// Create all the letters button
	for (int i{}; i < 6; ++i)
	{
		for (int j{}; j < 6; ++j)
		{
			const int index = i * 6 + j;
			m_ButtonsLetter[index] = std::make_unique<ButtonComponent>(m_Alphabet[index] + "Button", startPos,
			                                                           m_Alphabet[index], font);
			height = m_ButtonsLetter[index]->GetBox().height;
			m_ButtonsLetter[index]->SetOnButtonClick(this, &KeyboardComponent::AddLetter);
			startPos.x += width;
		}
		startPos.y += height;
		startPos.x = 40.f;
	}
}

void KeyboardComponent::RenderElement() const
{
	for (auto& button : m_ButtonsLetter)
	{
		button->RenderElement();
	}
}

void KeyboardComponent::UpdateElement()
{
	for(const auto& button : m_ButtonsLetter)
	{
		button->UpdateElement();
	}
}

void KeyboardComponent::DeletePreviousLetter()
{
	if (m_CurrentWord->empty())
	{
		return;
	}
	m_CurrentWord->pop_back();
}

void KeyboardComponent::AddLetter()
{
	const std::string letter{m_Alphabet[m_CurrentSelected]};
	*m_CurrentWord += letter;
	std::cout << m_CurrentWord << std::endl;
}

void KeyboardComponent::SelectTop()
{
	m_PreviousSelected = m_CurrentSelected;
	m_CurrentSelected -= 6;
	if (m_CurrentSelected < 0)
	{
		m_CurrentSelected += 36;
	}
}

void KeyboardComponent::SelectDown()
{
	m_PreviousSelected = m_CurrentSelected;
	m_CurrentSelected += 6;
	if (m_CurrentSelected >= 36)
	{
		m_CurrentSelected -= 36;
	}
}

void KeyboardComponent::SelectLeft()
{
	m_PreviousSelected = m_CurrentSelected;
	--m_CurrentSelected;
	if (m_CurrentSelected < 0)
	{
		m_CurrentSelected = 35;
	}
}

void KeyboardComponent::SelectRight()
{
	m_PreviousSelected = m_CurrentSelected;
	++m_CurrentSelected;
	if (m_CurrentSelected >= 36)
	{
		m_CurrentSelected = 0;
	}
}

void KeyboardComponent::SelectButton() const
{
	m_ButtonsLetter[m_PreviousSelected]->DeselectButton();
	m_ButtonsLetter[m_CurrentSelected]->SelectButton();
}

void KeyboardComponent::OnClick(const glm::vec2& vec)
{
	for(int i{}; i < 36; ++i)
	{
		m_CurrentSelected = i;
		m_ButtonsLetter[i]->OnClick(vec);
	}
}

std::string KeyboardComponent::GetCurrentWord() const
{
	return *m_CurrentWord;
}

void KeyboardComponent::MoveSelection(const glm::ivec2& vec)
{
	if(vec.x == 1)
	{
		SelectRight();
		SelectButton();
	}
	else if(vec.x == -1)
	{
		SelectLeft();
		SelectButton();
	}
	else if(vec.y == 1)
	{
		SelectDown();
		SelectButton();
	}
	else if(vec.y == -1)
	{
		SelectTop();
		SelectButton();
	}

}

void KeyboardComponent::OnPressed() const
{
	m_ButtonsLetter[m_CurrentSelected]->OnPressed();
}

void KeyboardComponent::SaveEntry()
{
	if(m_OnSaveEntryFunc)
	{
		m_OnSaveEntryFunc(*m_CurrentWord);
		m_OnSaveEntryFunc = nullptr;
	}
}

void KeyboardComponent::OnHover(const glm::vec3& vec)
{
	for (auto& button : m_ButtonsLetter)
	{
		button->OnHover(vec);
	}
}

void KeyboardComponent::BindText(std::string* pText)
{
	m_CurrentWord = pText;
}
