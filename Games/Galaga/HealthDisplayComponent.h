#pragma once
#include "Component.h"
#include "Observer.h"
#include "Event.h"

namespace dae 
{
	class GameActor;
	class HealthComponent;
	class TextComponent;

	class HealthDisplayComponent final : public Component, public Observer
	{
	public:
		HealthDisplayComponent(GameObject* owner, GameActor* actor);

		void Update() override {};
		void Notify(Event event) override;
	private:
		GameActor* m_pActor{};
		HealthComponent* m_pHealthComponent{};
		TextComponent* m_pTextComponent{};
	};

}