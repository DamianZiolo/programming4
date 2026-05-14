#include "Enemy.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include <GameActor.h>
#include <iostream>
#include <ProjectileComponent.h>
#include <Windows.h>
#include <sstream>

dae::Enemy::Enemy(GameObject* owner):Component(owner)
{
}


void dae::Enemy::TakeDamage()
{
	m_Health--;

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
	GetOwner()->MarkForRemoval();
}

void dae::Enemy::SetSlot(GameObject* slotPointer)
{
	m_pSlot = slotPointer;
}
