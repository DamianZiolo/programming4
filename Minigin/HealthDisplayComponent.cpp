#include "HealthDisplayComponent.h"
#include "GameActor.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "TextComponent.h"
#include "Event.h"


dae::HealthDisplayComponent::HealthDisplayComponent(GameObject* owner, GameActor* actor): Component(owner)
, m_pActor{ actor }
{ 
	m_pActor->AddObserver(this);
	m_pHealthComponent = m_pActor->GetOwner()->GetComponent<HealthComponent>();
	m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
}

void dae::HealthDisplayComponent::Notify(Event event)
{

	if (event == Event::PlayerDamaged)
	{
		int lives = m_pHealthComponent->GetHealth();
		m_pTextComponent->SetText("Health: " + std::to_string(lives));
	}
	else if (event == Event::PlayerDied)
	{
		m_pTextComponent->SetText("Health: 0 Player Died");
	}

}

