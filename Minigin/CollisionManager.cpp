#include "CollisionManager.h"

#include "BoxCollider.h"
#include "GameObject.h"
#include <algorithm>

void dae::CollisionManager::RegisterCollider(BoxCollider* collider)
{
	if (collider == nullptr)
		return;
	//Find looks if we hace collider in m_colliders and if no then return m_coliders.end
	const auto it = std::find(m_Colliders.begin(), m_Colliders.end(), collider);
	if (it == m_Colliders.end())
	{
		m_Colliders.push_back(collider);
	}
}

void dae::CollisionManager::UnregisterCollider(BoxCollider* collider)
{
	const auto it = std::remove(m_Colliders.begin(), m_Colliders.end(), collider);
	m_Colliders.erase(it, m_Colliders.end());
}

void dae::CollisionManager::Clear()
{
	m_Colliders.clear();
}

void dae::CollisionManager::Shutdown()
{
	m_IsShuttingDown = true;
	Clear();
}


void dae::CollisionManager::Update()
{
	for (size_t i = 0; i < m_Colliders.size(); ++i)
	{
		BoxCollider* first = m_Colliders[i];
		if (first == nullptr || !first->IsActive())
		{
			continue;
		}

		for (size_t j = i + 1; j < m_Colliders.size(); ++j)
		{
			BoxCollider* second = m_Colliders[j];
			if (second == nullptr || !second->IsActive())
			{
				continue;
			}

			if (first->IsCollidingWith(*second))
			{
				first->GetOwner()->OnCollisionEnter(second);
				second->GetOwner()->OnCollisionEnter(first);
			}

		}

	}

}

