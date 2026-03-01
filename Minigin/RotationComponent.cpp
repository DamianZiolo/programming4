#include "GameTime.h"
#include "GameObject.h"
#include "RotationComponent.h"
#include <iostream>

dae::RotationComponent::RotationComponent(GameObject* owner, float radius, float angularSpeed, bool clockwise, float phaseRad):
	Component(owner)
	, m_radius(radius)
	, m_angularSpeed(angularSpeed)
	, m_clockwise(clockwise)
	, m_phaseRad(phaseRad)
{
	center = GetOwner()->GetLocalPosition();
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

	
	const float x = center.x + m_radius * std::cos(m_phaseRad);
	const float y = center.y + m_radius * std::sin(m_phaseRad);

	GetOwner()->SetLocalPosition({ x, y, center.z });

	GetOwner()->SetLocalPosition(glm::vec3{ x, y, 0.0f });
}
