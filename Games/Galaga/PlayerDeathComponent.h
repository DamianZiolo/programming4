#pragma once

#include "Component.h"
#include "Observer.h"
#include "Event.h"

namespace dae
{
	class GameActor;

	class PlayerDeathComponent final : public Component, public Observer
	{
	public:
		PlayerDeathComponent(GameObject* owner, GameActor* actor);
		~PlayerDeathComponent() override = default;

		void Update() override {}
		void Notify(Event event) override;

	private:
		void DisablePlayer();

		GameActor* m_pActor{};
		bool m_IsDead{ false };
	};
}