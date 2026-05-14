#include "Enemy.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include <GameActor.h>
#include <iostream>

dae::Enemy::Enemy(GameObject* owner):Component(owner)
{
}

void dae::Enemy::OnCollisionEnter(BoxCollider* other)
{
	if (other->GetOwner()->GetComponent<GameActor>())
	{
		std::cout << "Enemy.cpp: I'm dying!" << std::endl;
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
