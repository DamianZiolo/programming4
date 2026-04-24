#pragma once
#include "Singleton.h"
#include <vector>

namespace dae
{
	class BoxCollider;

	class CollisionManager final : public Singleton<CollisionManager>
	{
	public:
		void RegisterCollider(BoxCollider* collider);
		void UnregisterCollider(BoxCollider* collider);

		void Update();

	private:
		std::vector<BoxCollider*> m_Colliders;
	};

}