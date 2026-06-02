#pragma once

#include "Component.h"

namespace dae
{
	class BoxCollider;

	class PlayerCollisionDamageComponent final : public Component
	{
	public:
		explicit PlayerCollisionDamageComponent(GameObject* owner);

		void Update() override;
		void OnCollisionEnter(BoxCollider* other) override;
		float m_BeamSoundCooldown{};
	};
}