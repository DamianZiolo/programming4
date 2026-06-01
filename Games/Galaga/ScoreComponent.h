#pragma once

#include "Component.h"
#include "Observer.h"
#include "EnemyType.h"
#include "Event.h"

namespace dae
{
	class ScoreComponent final : public Component, public Observer
	{
	public:
		explicit ScoreComponent(GameObject* owner);

		void Update() override {}

		void Notify(Event event) override;
		void Notify(Event event, GameObject* gameObject) override;

		void AddScore(int amount);
		int GetScore() const;

		void ResetScore() { m_Score = 0; }

	private:
		int GetScoreForEnemy(EnemyType enemyType, bool isInFormation) const;

		int m_Score{};
	};
}