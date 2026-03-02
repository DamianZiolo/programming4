#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae 
{
	class RotationComponent final : public Component
	{
	public:
		RotationComponent(GameObject* owner,
			float radius,
			float angularSpeed,
			bool clockwise = false,
			float phaseRad = 0.0f);

		void Update() override;

	private:
		float m_radius{ 0.0f };
		float m_angularSpeed{ 0.0f };
		bool  m_clockwise{ false };
		float m_phaseRad{ 0.0f }; // radians
		glm::vec3 m_center{ 0.0f, 0.0f, 0.0f };
	};
}