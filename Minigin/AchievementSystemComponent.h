#pragma once
#include "Component.h"
#include "Observer.h"
namespace dae
{
	class GameActor;

	class AchievementSystemComponent final : public Component, public Observer
	{
		public:
			AchievementSystemComponent(GameObject* owner);
			
		void Update() override {};
		void Notify(Event event) override;
		void AddActor(GameActor* actor);

	private:
		bool m_ScoreReached500Unclocked{ false };
	};
}