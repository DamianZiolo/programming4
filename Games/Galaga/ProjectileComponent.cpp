#include "ProjectileComponent.h"
#include "GameObject.h"
#include "Transform.h"

#include "BoxCollider.h"
#include <Enemy.h>
#include <Windows.h>
#include <GameTime.h>

dae::ProjectileComponent::ProjectileComponent(GameObject* owner):
	Component(owner),
	m_pOwner{owner}
{
}

void dae::ProjectileComponent::OnCollisionEnter(BoxCollider* other)
{
	if (m_HasHit)
		return;


	auto* enemy = other->GetOwner()->GetComponent<Enemy>();
	if (enemy)
	{
		m_HasHit = true;
		enemy->TakeDamage();
		OutputDebugStringA("Projectile hit\n");

		if (auto* col = GetOwner()->GetComponent<BoxCollider>())
			col->SetActive(false);

		GetOwner()->MarkForRemoval();
	}

}

void dae::ProjectileComponent::Update()
{
	auto position = m_pOwner->GetLocalPosition();

	position += m_Velocity *
		dae::GameTime::GetIntance().GetDeltaTime();

	m_pOwner->SetLocalPosition(position);
}