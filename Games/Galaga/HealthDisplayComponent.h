#pragma once
#include "Component.h"
#include "Observer.h"
#include "Event.h"
#include <vector>
#include <Scene.h>

namespace dae 
{
	class GameActor;
	class HealthComponent;
	class TextComponent;

	class HealthDisplayComponent final : public Component, public Observer
	{
	public:
		HealthDisplayComponent(GameObject* owner, GameActor* actor, Scene& scene, int maxLives = 4);

		void Update() override {};
		void Notify(Event event) override;
	private:
		void RefreshLives();

		GameActor* m_pActor{};
		HealthComponent* m_pHealthComponent{};
		std::vector<GameObject*> m_LifeIcons{};
	};

}