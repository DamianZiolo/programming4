#pragma once
#include "Component.h"

namespace dae
{
	class ScoreComponent final : public Component
	{
	public:
		explicit ScoreComponent(GameObject* owner);

		void Update() override {};

		void AddScore(int amount);
		int GetScore() const;

		void ResetScore() { m_Score = 0; }

	private:
		int m_Score{};
	};
}