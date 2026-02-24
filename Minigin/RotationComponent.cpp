#include "GameTime.h"
#include "GameObject.h"
#include "RotationComponent.h"
#include <iostream>

dae::RotationComponent::RotationComponent(GameObject* owner, glm::vec3 orbitPoint, float radius, float angularSpeed, bool clockwise, float phaseRad):
	Component(owner)
	, m_orbitPoint(orbitPoint)
	, m_radius(radius)
	, m_angularSpeed(angularSpeed)
	, m_clockwise(clockwise)
	, m_phaseRad(phaseRad)
{
}

void dae::RotationComponent::Update()
{
	std::cout << "Test" << std::endl;
	if (!IsActive()) return;

	const float deltaTime = GameTime::GetIntance().GetDeltaTime();
	if (m_clockwise)
	{
		m_phaseRad += m_angularSpeed * deltaTime;
	}
	else
	{
		m_phaseRad -= m_angularSpeed * deltaTime;
	}

	const float x = m_orbitPoint.x + m_radius * cos(m_phaseRad);
	const float y = m_orbitPoint.y + m_radius * sin(m_phaseRad);

	GetOwner()->SetLocalPosition(glm::vec3{ x, y, 0.0f });
}
