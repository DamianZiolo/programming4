#include "HealthComponent.h"
#include "GameObject.h"
#include "GameActor.h"
#include "Event.h"
#include <stdexcept>
#include "PlayerInvulnerabilityComponent.h"

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
	auto* invulnerabilititi = GetOwner()->GetComponent<PlayerInvulnerabilityComponent>();

	if (invulnerabilititi->IsInvulnerable())
		return;

	m_CurrentHealth -= 1;

	if (m_CurrentHealth <= 0)
	{
		m_CurrentHealth = 0;
		GetOwner()->GetComponent<dae::GameActor>()->NotifyObservers(Event::PlayerDied);
		return;
	}

	GetOwner()->GetComponent<dae::GameActor>()->NotifyObservers(Event::PlayerDamaged);

	
		
}

int dae::HealthComponent::GetHealth() const 
{
	return  m_CurrentHealth;
}
