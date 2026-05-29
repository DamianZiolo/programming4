#pragma once
#include "Component.h"

namespace dae
{
	class BoxCollider;
	class ProjectilePoolComponent;
	//This is pure virtual component because Update is not ovveriten, so it's still Update()=0;
	class Enemy : public Component
	{
	public:
		explicit Enemy(GameObject* owner, ProjectilePoolComponent& projectilePool);
		~Enemy() override = default;
		virtual void TakeDamage();

		Enemy(const Enemy&) = delete;
		Enemy(Enemy&&) = delete;
		Enemy& operator=(const Enemy&) = delete;
		Enemy& operator=(Enemy&&) = delete;
		void Die();
		void SetSlot(GameObject* slotPointer);
		GameObject* GetSlot() { return m_pSlot; }
		ProjectilePoolComponent& GetProjectilePool(){return m_pProjectilePool;}
	private:
		GameObject* m_pSlot;
	protected:
		int m_Health{};
		ProjectilePoolComponent& m_pProjectilePool;
	};

}