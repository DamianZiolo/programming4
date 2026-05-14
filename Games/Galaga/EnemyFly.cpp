#include "EnemyFly.h"
#include <iostream>
#include <GameActor.h>
#include "BoxCollider.h"
#include "GameObject.h"
#include <windows.h>
#include <sstream>
#include "ServiceLocator.h"
#include <ProjectileComponent.h>

dae::EnemyFly::EnemyFly(GameObject* owner):Enemy(owner)
{
	m_Health = 1;
}

void dae::EnemyFly::Update()
{
	
}

