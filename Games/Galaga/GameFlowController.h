#pragma once

#include "Component.h"
#include "Observer.h"
#include "Event.h"
#include "GameSessionStats.h"
#include <vector>

namespace dae
{
	class GameActor;
	class LevelManagerComponent;
	class ScoreComponent;

	class GameFlowController final : public Component, public Observer
	{
	public:
		GameFlowController(
			GameObject* owner,
			LevelManagerComponent* levelManager,
			ScoreComponent* scoreComponent);

		void Update() override;
		void Notify(Event event) override;

		void AddPlayer(GameActor* player);

	private:
		bool AreAllPlayersDead() const;
		void GoToEndScreen();

		LevelManagerComponent* m_pLevelManager{};

		std::vector<GameActor*> m_Players{};
		std::vector<bool> m_PlayerDead{};

		bool m_HasEnded{ false };
		ScoreComponent* m_pScoreComponent{};
	};
}