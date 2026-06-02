#include "PlayerCollisionDamageComponent.h"

#include "GameObject.h"
#include "BoxCollider.h"
#include "Enemy.h"
#include "HealthComponent.h"
#include "TractorBeamComponent.h"
#include "ServiceLocator.h"
#include "GameTime.h"

dae::PlayerCollisionDamageComponent::PlayerCollisionDamageComponent(GameObject* owner)
	: Component(owner)
{
}

void dae::PlayerCollisionDamageComponent::Update()
{
	if (m_BeamSoundCooldown > 0.f)
	{
		m_BeamSoundCooldown -= GameTime::GetIntance().GetDeltaTime();
	}
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
	if (hitTractorBeam)
	{
		if (m_BeamSoundCooldown <= 0.f)
		{
			if (ServiceLocator::GetSoundSystem().IsMuted() != true)
			{
				ServiceLocator::GetSoundSystem().Play(7, 0.1f);
				m_BeamSoundCooldown = 3.f;
			}
		}
	}

	auto* health = GetOwner()->GetComponent<HealthComponent>();

	if (!health)
		return;

	health->DealDamage();
}