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
		float m_radius{ 0.0f };
		bool m_clockwise{ false };
		float m_radius{ 0.0f };
		float m_phaseRad{ 0.0f }; //radians - where is object on the circle at the moment (0 means to the right of the orbit point, PI/2 means above the orbit point)
		glm::vec3 center{ 0.0f, 0.0f, 0.0f };
	};
}