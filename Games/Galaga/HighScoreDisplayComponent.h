#pragma once

#include "Component.h"

#include <vector>

namespace dae
{
	class TextComponent;

	class HighScoreDisplayComponent final : public Component
	{
	public:
		explicit HighScoreDisplayComponent(GameObject* owner);

		void Update() override;

		void AddText(TextComponent* text);

	private:
		void Refresh();

		std::vector<TextComponent*> m_Texts{};
		bool m_HasRefreshed{ false };
	};
}