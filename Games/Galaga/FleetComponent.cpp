#include "FleetComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#define NOMINMAX
#include <windows.h>
#include <sstream>
#include "EnemyFly.h"
#include "EnemyButterfly.h"
#include <algorithm>
#include <cstdlib>
#include <cmath>

dae::FleetComponent::FleetComponent(GameObject* owner,int rows,int cols,float spacingX,float spacingY)
	: Component(owner)
	, m_Rows{ rows }
	, m_Cols{ cols }
	, m_SpacingX{ spacingX }
	, m_SpacingY{ spacingY }
	, m_StartPos{ GetOwner()->GetLocalPosition() }
	, m_Time{0.f}
{
	m_Slots.resize(rows * cols);
}

void dae::FleetComponent::Update()
{
	m_Time += dae::GameTime::GetIntance().GetDeltaTime();
	float offsetX = sin(m_Time) * 20.f +40.f;

	GetOwner()->SetLocalPosition(
		glm::vec3(offsetX, 0, 0.f)
	);

	m_AttackTimer += GameTime::GetIntance().GetDeltaTime();

	if (m_AttackTimer < m_AttackInterval)
		return;

	m_AttackTimer = 0.f;

	int currentAttackers = 0;
	std::vector<Enemy*> availableEnemies{};

	for (auto& slot : m_Slots)
	{
		if (!slot.object)
			continue;

		for (auto* child : slot.object->GetChildren())
		{
			auto* fly = child->GetComponent<EnemyFly>();
			auto* butterfly = child->GetComponent<EnemyButterfly>();

			if (!fly && !butterfly)
				continue;

			auto* enemy = child->GetComponent<Enemy>();

			if (!enemy)
				continue;

			if (!enemy->IsAttackRequested())
			{
				availableEnemies.push_back(enemy);
			}
		}
	}

	const int freeAttackSlots = m_MaxAttackers - currentAttackers;

	if (freeAttackSlots <= 0)
		return;

	const int attackCount = std::min(m_MaxAttackers, static_cast<int>(availableEnemies.size()));

	for (int i = 0; i < attackCount; ++i)
	{
		const int randomIndex = rand() % availableEnemies.size();

		availableEnemies[randomIndex]->RequestAttack();

		availableEnemies.erase(
			availableEnemies.begin() + randomIndex
		);
	}

}

void dae::FleetComponent::SetSlot(int row, int col, GameObject* slot)
{
	m_Slots[row * m_Cols + col].object = slot;
	m_Slots[row * m_Cols + col].occupied = false;
}

dae::GameObject* dae::FleetComponent::GetSlot(int row, int col)
{
	return m_Slots[row * m_Cols + col].object;
}

glm::vec3 dae::FleetComponent::GetSlotWorldPosition(int row, int col)
{
	return m_Slots[row * m_Cols + col].object->GetWorldPosition();
}