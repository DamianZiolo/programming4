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
		void Clear();
		void Shutdown();


		bool IsShuttingDown() const { return m_IsShuttingDown; }

		void Update();

	private:
		std::vector<BoxCollider*> m_Colliders;
		bool m_IsShuttingDown{ false };
	};

}