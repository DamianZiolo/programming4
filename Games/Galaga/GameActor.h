#pragma once
#include <vector>
#include "Component.h"
#include "Event.h"

namespace dae
{
	class Observer;
	class GameObject;
	class GameActor : public Component
	{
	public:
		explicit GameActor(GameObject* owner);
		void Update() override {};
		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);
		void NotifyObservers(Event event);
	private:
		std::vector<Observer*> m_observers;
	};
}