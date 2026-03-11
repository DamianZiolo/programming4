#pragma once
#include <glm/vec3.hpp>
#include "Component.h"

namespace dae 
{
	class MovementComponent final : public Component
	{
	public:
		MovementComponent(GameObject* owner, float speed);

		void Update() override;
		void Move(const glm::vec3& direction);

		float GetSpeed() const { return m_Speed; }
		void SetSpeed(float speed) { m_Speed = speed; }

	private:
		float m_Speed{};
	};

}