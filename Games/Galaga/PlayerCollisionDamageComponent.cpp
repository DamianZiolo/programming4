#include "PlayerCollisionDamageComponent.h"

#include "GameObject.h"
#include "BoxCollider.h"
#include "Enemy.h"
#include "HealthComponent.h"
#include "TractorBeamComponent.h"

dae::PlayerCollisionDamageComponent::PlayerCollisionDamageComponent(GameObject* owner)
	: Component(owner)
{
}

void dae::PlayerCollisionDamageComponent::OnCollisionEnter(BoxCollider* other)
{
	if (!other)
		return;

	auto* otherOwner = other->GetOwner();

	if (!otherOwner)
		return;

	const bool hitEnemy =
		otherOwner->GetComponent<Enemy>() != nullptr;

	const bool hitTractorBeam =
		otherOwner->GetComponent<TractorBeamComponent>() != nullptr;

	if (!hitEnemy && !hitTractorBeam)
		return;

	auto* health = GetOwner()->GetComponent<HealthComponent>();

	if (!health)
		return;

	health->DealDamage();
}