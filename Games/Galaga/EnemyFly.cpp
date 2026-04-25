#include "EnemyFly.h"
#include <iostream>
#include <GameActor.h>
#include "BoxCollider.h"
#include "GameObject.h"
#include <windows.h>
#include <sstream>

dae::EnemyFly::EnemyFly(GameObject* owner):Enemy(owner)
{
}

void dae::EnemyFly::Update()
{
	
}

void dae::EnemyFly::OnCollisionEnter(BoxCollider* other)
{
	auto* myCollider = GetOwner()->GetComponent<BoxCollider>();

	auto myPos = myCollider->GetPosition();
	auto otherPos = other->GetPosition();
	
	std::stringstream ss;
	ss << "Enemy collision!\n";
	ss << "My collider pos: " << myPos.x << ", " << myPos.y << "\n";
	ss << "Other collider pos: " << otherPos.x << ", " << otherPos.y << "\n";

	OutputDebugStringA(ss.str().c_str());

	auto* actor = other->GetOwner()->GetComponent<GameActor>();
	if (actor)
	{
		OutputDebugStringA("EnemyFly.cpp: I'm dying!\n");
		Die();
	}

}
