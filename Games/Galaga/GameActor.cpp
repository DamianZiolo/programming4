#include "GameActor.h"
#include <algorithm>

#include "Observer.h"
#include "Component.h"
#include "GameObject.h"

dae::GameActor::GameActor(GameObject* owner): Component(owner)
{
}

void dae::GameActor::AddObserver(Observer* observer)
{
	m_observers.emplace_back(observer);
}

void dae::GameActor::RemoveObserver(Observer* observer)
{
	//so Remove is not changing the size of vector, it just moves the elements that are not equal to observer to the front of the vector, and returns an iterator to the new end of the vector, and then we erase the "removed" elements from the end of the vector, so we are not invalidating any iterators that are pointing to the elements that are not removed, and we are not leaving any "holes" in the vector, and we are not changing the size of the vector until we call erase, so we can safely iterate over the vector while removing elements from it, and we can safely remove multiple observers at once if they are the same, and we can safely remove observers that are not in the vector without causing any issues
	m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer), m_observers.end());
}

void dae::GameActor::NotifyObservers(Event event)
{
	for (auto observer : m_observers)
	{
		observer->Notify(event);
	}
}


