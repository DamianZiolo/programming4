#include "MenuComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include <SceneManager.h>

dae::MenuComponent::MenuComponent(GameObject* owner):Component(owner)
{
}


void dae::MenuComponent::AddOption(dae::TextComponent* text)
{
	if(text == nullptr)
	{
		return;
	}
	
	m_Options.push_back(text);

	RefreshColors();

}

void dae::MenuComponent::MoveUp()
{
	if (m_Options.empty())
		return;

	m_SelectedIndex--;

	if (m_SelectedIndex < 0)
	{
		m_SelectedIndex = static_cast<int>(m_Options.size()) - 1;
	}

	RefreshColors();
}

void dae::MenuComponent::MoveDown()
{
	if (m_Options.empty())
		return;

	m_SelectedIndex++;

	if (m_SelectedIndex >= static_cast<int>(m_Options.size()))
	{
		m_SelectedIndex = 0;
	}

	RefreshColors();
}

void dae::MenuComponent::Confirm()
{
	auto& sceneManager = dae::SceneManager::GetInstance();
	switch (m_SelectedIndex)
	{
	case 0:
		// Single mode
		sceneManager.SetActiveScene(1);
		break;

	case 1:
		// Co-op mode
		sceneManager.SetActiveScene(1);
		break;

	case 2:
		// Versus mode
		sceneManager.SetActiveScene(1);
		break;

	default:
		break;
	}
}

void dae::MenuComponent::Update()
{
}

void dae::MenuComponent::RefreshColors()
{
	for (size_t i = 0; i < m_Options.size(); ++i)
	{
		if (i == m_SelectedIndex)
		{
			m_Options[i]->SetColor(SDL_Color{ 255,255,0,255 });
		}
		else
		{
			m_Options[i]->SetColor(SDL_Color{ 255,255,255,255 });
		}
	}
}
