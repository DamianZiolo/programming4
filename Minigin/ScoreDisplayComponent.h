#pragma once
#include "Component.h"
#include "Observer.h"
#include "Event.h"

namespace dae
{
	class GameActor;
	class ScoreComponent;
	class TextComponent;

	class ScoreDisplayComponent final : public Component, public Observer
	{
	public:
		ScoreDisplayComponent(GameObject* owner, GameActor* actor);

		void Update() override {};
		void Notify(Event event) override;

	private:
		GameActor* m_pActor{};
		ScoreComponent* m_pScoreComponent{};
		TextComponent* m_pTextComponent{};
	};
}