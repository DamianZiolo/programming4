#include "MenuComponent.h"

#include "GameObject.h"
#include "TextComponent.h"
#include "SceneManager.h"
#include "GameSettings.h"
#include "GameMode.h"

#include <algorithm>
#include <string>

dae::MenuComponent::MenuComponent(GameObject* owner)
	: Component(owner)
{
}

void dae::MenuComponent::AddOption(dae::TextComponent* text)
{
	if (!text)
		return;

	MenuEntry entry{};
	entry.text = text;
	entry.type = EntryType::ModeOption;
	entry.modeIndex = static_cast<int>(m_ModeOptionCount);
	entry.currentCharacter = '\0';

	++m_ModeOptionCount;

	m_Entries.push_back(entry);

	RefreshColors();
}

void dae::MenuComponent::AddNameSlot(dae::TextComponent* text)
{
	if (!text)
		return;

	MenuEntry entry{};
	entry.text = text;
	entry.type = EntryType::NameSlot;
	entry.modeIndex = -1;
	entry.currentCharacter = 'A';

	entry.text->SetText(std::string(1, entry.currentCharacter));

	m_Entries.push_back(entry);

	RefreshColors();
}

void dae::MenuComponent::MoveUp()
{
	if (m_Entries.empty())
		return;

	--m_SelectedIndex;

	if (m_SelectedIndex < 0)
	{
		m_SelectedIndex = static_cast<int>(m_Entries.size()) - 1;
	}

	RefreshColors();
}

void dae::MenuComponent::MoveDown()
{
	if (m_Entries.empty())
		return;

	++m_SelectedIndex;

	if (m_SelectedIndex >= static_cast<int>(m_Entries.size()))
	{
		m_SelectedIndex = 0;
	}

	RefreshColors();
}

void dae::MenuComponent::MoveLeft()
{
	ChangeCurrentLetter(-1);
}

void dae::MenuComponent::MoveRight()
{
	ChangeCurrentLetter(1);
}

void dae::MenuComponent::Confirm()
{
	if (m_Entries.empty())
		return;

	const auto& selectedEntry = m_Entries[m_SelectedIndex];

	if (selectedEntry.type != EntryType::ModeOption)
		return;

	GameSettings::GetInstance().SetPlayerName(GetName());

	auto& sceneManager = dae::SceneManager::GetInstance();

	switch (selectedEntry.modeIndex)
	{
	case 0:
		GameSettings::GetInstance().SetGameMode(GameMode::Solo);
		sceneManager.SetActiveScene(1);
		break;

	case 1:
		GameSettings::GetInstance().SetGameMode(GameMode::Duo);
		sceneManager.SetActiveScene(1);
		break;

	case 2:
		GameSettings::GetInstance().SetGameMode(GameMode::Versus);
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
	for (int i = 0; i < static_cast<int>(m_Entries.size()); ++i)
	{
		auto* text = m_Entries[i].text;

		if (!text)
			continue;

		if (i == m_SelectedIndex)
		{
			text->SetColor(SDL_Color{ 255,255,0,255 });
		}
		else
		{
			text->SetColor(SDL_Color{ 255,255,255,255 });
		}
	}
}

void dae::MenuComponent::ChangeCurrentLetter(int direction)
{
	if (m_Entries.empty())
		return;

	auto& selectedEntry = m_Entries[m_SelectedIndex];

	if (selectedEntry.type != EntryType::NameSlot)
		return;

	const std::string characters{ AvailableCharacters };

	auto currentIt = std::find(
		characters.begin(),
		characters.end(),
		selectedEntry.currentCharacter);

	int currentIndex{};

	if (currentIt != characters.end())
	{
		currentIndex = static_cast<int>(
			std::distance(characters.begin(), currentIt));
	}

	currentIndex += direction;

	if (currentIndex < 0)
	{
		currentIndex = static_cast<int>(characters.size()) - 1;
	}
	else if (currentIndex >= static_cast<int>(characters.size()))
	{
		currentIndex = 0;
	}

	selectedEntry.currentCharacter = characters[currentIndex];

	if (selectedEntry.text)
	{
		selectedEntry.text->SetText(
			std::string(1, selectedEntry.currentCharacter));
	}
}

std::string dae::MenuComponent::GetName() const
{
	std::string result{};

	for (const auto& entry : m_Entries)
	{
		if (entry.type == EntryType::NameSlot)
		{
			result += entry.currentCharacter;
		}
	}

	return result;
}