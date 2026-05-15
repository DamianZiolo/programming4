#pragma once
#include "Component.h"
#include <vector>

namespace dae
{
	class GameObject;
	class TextComponent;

	class MenuComponent final : public dae::Component
	{
	public:
		MenuComponent(GameObject* owner);

		void AddOption(dae::TextComponent* text);
		void MoveUp();
		void MoveDown();
		void Confirm();

		void Update() override;

	private:
		void RefreshColors();
		std::vector<dae::TextComponent*> m_Options{};
		int m_SelectedIndex{ 0 };

	};
}