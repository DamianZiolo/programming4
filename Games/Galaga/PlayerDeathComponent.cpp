#include "PlayerDeathComponent.h"

#include "GameActor.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "BoxCollider.h"

dae::PlayerDeathComponent::PlayerDeathComponent(
	GameObject* owner,
	GameActor* actor)
	: Component(owner)
	, m_pActor{ actor }
{
	if (m_pActor)
	{
		m_pActor->AddObserver(this);
	}
}

void dae::PlayerDeathComponent::Notify(Event event)
{
	if (event != Event::PlayerDied)
		return;

	if (m_IsDead)
		return;

	m_IsDead = true;

	DisablePlayer();
}

void dae::PlayerDeathComponent::DisablePlayer()
{
	auto* owner = GetOwner();

	if (!owner)
		return;

	if (auto* render = owner->GetComponent<RenderComponent>())
	{
		render->SetActive(false);
	}

	if (auto* collider = owner->GetComponent<BoxCollider>())
	{
		collider->SetActive(false);
	}
	
}