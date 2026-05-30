#include "ProjectileComponent.h"
#include "GameObject.h"
#include "Transform.h"

#include "BoxCollider.h"
#include <Enemy.h>
#include <Windows.h>
#include <GameTime.h>
#include <RenderComponent.h>
#include <GameActor.h>
#include <HealthComponent.h>

dae::ProjectileComponent::ProjectileComponent(GameObject* owner):
	Component(owner),
	m_pOwner{owner}
{
}

void dae::ProjectileComponent::OnCollisionEnter(BoxCollider* other)
{
	if (!m_IsInUse || m_HasHit)
		return;

	auto* otherOwner = other->GetOwner();

	if (m_OwnerType == ProjectileOwner::Player)
	{
		auto* enemy = otherOwner->GetComponent<Enemy>();
		if (!enemy)
			return;

		m_HasHit = true;
		enemy->TakeDamage();
		Deactivate();
	}
	else if (m_OwnerType == ProjectileOwner::Enemy)
	{
		auto* player = otherOwner->GetComponent<GameActor>();
		if (!player)
			return;

		m_HasHit = true;

		if (auto* health = otherOwner->GetComponent<HealthComponent>())
			health->DealDamage(); 

		Deactivate();
	}
}

void dae::ProjectileComponent::Update()
{
	if (!m_IsInUse)
		return;

	auto position = m_pOwner->GetLocalPosition();

	position += m_Velocity *
		dae::GameTime::GetIntance().GetDeltaTime();
	m_pOwner->SetLocalPosition(position);

	m_LifeTime += dae::GameTime::GetIntance().GetDeltaTime();
	if (m_LifeTime >= m_MaxLifeTime)
		Deactivate();
}

void dae::ProjectileComponent::SetVelocity(float velocity)
{
	float randomX = 0;
	if (m_OwnerType == dae::ProjectileOwner::Enemy)
	{
		randomX = static_cast<float>((rand() % 201) - 100);
	}
	m_Velocity = glm::vec3(randomX, velocity, 0.f);
}

void dae::ProjectileComponent::Activate(const glm::vec3& position, float velocity, ProjectileOwner ownerType)
{
	m_IsInUse = true;
	m_HasHit = false;
	m_OwnerType = ownerType;
	m_LifeTime = 0.f;
	SetVelocity(velocity);
	GetOwner()->SetLocalPosition(position);

	if (auto* colider = GetOwner()->GetComponent<BoxCollider>())
	{
		colider->SetActive(true);
	}
	if (auto* render = GetOwner()->GetComponent<RenderComponent>())
	{
		if (m_OwnerType == dae::ProjectileOwner::Player)
		{
			render->SetTexture("Bullet.png");
		}
		else 
		{
			render->SetTexture("EnemyBullet.png");
		}
		render->SetActive(true);
	}
		
}

void dae::ProjectileComponent::Deactivate()
{
	m_IsInUse = false;

	if (auto* col = GetOwner()->GetComponent<BoxCollider>())
		col->SetActive(false);

	if (auto* render = GetOwner()->GetComponent<RenderComponent>())
		render->SetActive(false);
}

bool dae::ProjectileComponent::IsInUse() const
{
	return m_IsInUse;
}

dae::ProjectileOwner dae::ProjectileComponent::GetProjectileOwner() const
{
	return m_OwnerType;
}
