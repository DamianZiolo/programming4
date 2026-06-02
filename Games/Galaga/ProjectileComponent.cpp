#include "ProjectileComponent.h"
#include "GameObject.h"
#include "Transform.h"

#include "BoxCollider.h"
#include <Enemy.h>
#include <Windows.h>
#include <GameTime.h>
#include <RenderComponent.h>
#include <HealthComponent.h>
#include "GameSettings.h"
#include <PlayerShootingComponent.h>
#include "GameSessionStats.h"

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

		GameSessionStats::GetInstance().AddHit();

		enemy->TakeDamage();
		Deactivate();
	}
	else if (m_OwnerType == ProjectileOwner::Enemy)
	{
		auto* player = otherOwner->GetComponent<PlayerShootingComponent>();
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

	const float dt = GameTime::GetIntance().GetDeltaTime();

	m_Timer += dt;

	if (m_Timer > 7.f)
	{
		Deactivate();
		return;
	}

	auto position = m_pOwner->GetLocalPosition();

	position += m_Velocity * dt;

	m_pOwner->SetLocalPosition(position);

	if (IsOutsideScreen())
	{
		Deactivate();
		return;
	}
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
	SetVelocity(velocity);
	GetOwner()->SetLocalPosition(position);

	m_Timer = 0.f;

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
	m_HasHit = false;
	m_Timer = 0.f;
	m_Velocity = glm::vec3{ 0.f, 0.f, 0.f };

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

bool dae::ProjectileComponent::IsOutsideScreen() const
{
	const auto worldPosition = GetOwner()->GetWorldPosition();

	constexpr float margin = 30.f;

	return worldPosition.y < -margin ||
		worldPosition.y > GameSettings::ScreenHeight + margin ||
		worldPosition.x < -margin ||
		worldPosition.x > GameSettings::ScreenWidth + margin;
}