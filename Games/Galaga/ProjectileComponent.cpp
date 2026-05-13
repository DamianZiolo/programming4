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
	auto* enemy = other->GetOwner()->GetComponent<Enemy>();
	if (enemy)
	{
		OutputDebugStringA("Projectile hit\n");
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