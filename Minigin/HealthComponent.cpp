#include "HealthComponent.h"
#include "GameObject.h"
#include "GameActor.h"
#include "Event.h"
#include <stdexcept>

dae::HealthComponent::HealthComponent(GameObject* owner, int startHealth) : Component(owner)
	,m_MaxHealth(startHealth)
	,m_CurrentHealth(startHealth)
{
	auto actor = GetOwner()->GetComponent<dae::GameActor>();

	if (!actor)
	{
		throw std::runtime_error("HealthComponent requires a GameActor component");
	}
}

void dae::HealthComponent::Update()
{
}

void dae::HealthComponent::DealDamage()
{
	m_CurrentHealth -= 1;
	GetOwner()->GetComponent<dae::GameActor>()->NotifyObservers(Event::PlayerDamaged);

	if (m_CurrentHealth <= 0)
	{
		m_CurrentHealth = 0;
		GetOwner()->GetComponent<dae::GameActor>()->NotifyObservers(Event::PlayerDied);
	}
		
}

int dae::HealthComponent::GetHealth() const 
{
	return  m_CurrentHealth;
}
