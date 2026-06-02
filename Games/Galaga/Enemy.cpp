#include "Enemy.h"

#include "BoxCollider.h"
#include "GameObject.h"
#include "GameTime.h"
#include "ProjectilePoolComponent.h"
#include "GameSettings.h"
#include "ServiceLocator.h"
#include <Windows.h>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <GameActor.h>

dae::Enemy::Enemy(GameObject* owner, ProjectilePoolComponent& projectilePool)
	: Component(owner)
	, m_pSlot{ nullptr }
	, m_pProjectilePool{ projectilePool }
{
}

void dae::Enemy::TakeDamage()
{
	--m_Health;

	std::stringstream ss;
	ss << "Enemy HP: " << m_Health << "\n";
	OutputDebugStringA(ss.str().c_str());

	if (m_Health <= 0)
	{
		Die();
	}
}

void dae::Enemy::Die()
{
	if (auto* actor = GetOwner()->GetComponent<GameActor>())
	{
		actor->NotifyObservers(Event::EnemyDied, GetOwner());
		if (ServiceLocator::GetSoundSystem().IsMuted() != true)
		{
			auto type = GetEnemyType();

			if (type == EnemyType::Boss)
			{
				ServiceLocator::GetSoundSystem().Play(6, 1.f);
			}
			else 
			{
				ServiceLocator::GetSoundSystem().Play(5, 1.f);
			}
			
		}
	}

	GetOwner()->MarkForRemoval();
}

void dae::Enemy::SetSlot(GameObject* slotPointer)
{
	m_pSlot = slotPointer;
}

void dae::Enemy::StartAttack()
{
	m_AttackRequested = false;
	m_IsAttacking = true;

	GetOwner()->SetParent(nullptr, true);
}

void dae::Enemy::FinishAttack()
{
	m_IsAttacking = false;
	m_AttackRequested = false;
}

bool dae::Enemy::TeleportToTopIfBelowScreen()
{
	auto* owner = GetOwner();

	auto pos = owner->GetWorldPosition();

	if (pos.y > GameSettings::ScreenHeight)
	{
		pos.y = -TopOffset;
		owner->SetWorldPosition(pos);
		return true;
	}

	return false;
}

bool dae::Enemy::ReturnToSlot(float speed)
{
	auto* owner = GetOwner();
	auto* slot = GetSlot();

	if (!owner || !slot)
		return false;

	const auto currentPos = owner->GetWorldPosition();
	const auto targetPos = slot->GetWorldPosition();

	auto direction = targetPos - currentPos;
	const auto distance = glm::length(direction);

	if (distance > 0.001f)
	{
		direction = glm::normalize(direction);

		const auto newPos =
			currentPos +
			direction *
			speed *
			GameTime::GetIntance().GetDeltaTime();

		owner->SetWorldPosition(newPos);
	}

	if (distance <= 2.f)
	{
		owner->SetParent(slot, false);
		owner->SetLocalPosition(0.f, 0.f, 0.f);

		FinishAttack();

		return true;
	}

	return false;
}