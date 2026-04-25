#pragma once
#include "Component.h"

namespace dae
{
	class BoxCollider;

	//This is pure virtual component because Update is not ovveriten, so it's still Update()=0;
	class Enemy : public Component
	{
	public:
		explicit Enemy(GameObject* owner);
		~Enemy() override = default;

		virtual void OnCollisionEnter(BoxCollider* other) override;

		Enemy(const Enemy&) = delete;
		Enemy(Enemy&&) = delete;
		Enemy& operator=(const Enemy&) = delete;
		Enemy& operator=(Enemy&&) = delete;

	protected:
		void Die();

	};

}