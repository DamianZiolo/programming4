#pragma once

#include "Component.h"

#include <string>
#include <vector>

namespace dae
{
	class TextComponent;

	class MenuComponent final : public Component
	{
	public:
		explicit MenuComponent(GameObject* owner);

		void Update() override;

		void AddOption(TextComponent* text);
		void AddNameSlot(TextComponent* text);

		void MoveUp();
		void MoveDown();
		void MoveLeft();
		void MoveRight();

		void Confirm();

	private:
		enum class EntryType
		{
			ModeOption,
			NameSlot
		};

		struct MenuEntry
		{
			TextComponent* text{};
			EntryType type{};
			int modeIndex{ -1 };
			char currentCharacter{ 'A' };
		};

		void RefreshColors();
		void ChangeCurrentLetter(int direction);
		std::string GetName() const;

		std::vector<MenuEntry> m_Entries{};

		int m_SelectedIndex{};
		int m_ModeOptionCount{};

		static constexpr const char* AvailableCharacters =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	};
}