#include "GameTime.h"
#include "GameObject.h"
#include "RotationComponent.h"
#include <iostream>
#include <cmath>

dae::RotationComponent::RotationComponent(GameObject* owner, float radius, float angularSpeed, bool clockwise, float phaseRad)
    : Component(owner)
    , m_radius(radius)
    , m_angularSpeed(angularSpeed)
    , m_clockwise(clockwise)
    , m_phaseRad(phaseRad)
{
    m_center = GetOwner()->GetLocalPosition();
}

void dae::RotationComponent::Update()
{
    if (!IsActive()) return;

    const float deltaTime = GameTime::GetIntance().GetDeltaTime();

    if (m_clockwise)
        m_phaseRad += m_angularSpeed * deltaTime;
    else
        m_phaseRad -= m_angularSpeed * deltaTime;

    const float x = m_center.x + m_radius * std::cos(m_phaseRad);
    const float y = m_center.y + m_radius * std::sin(m_phaseRad);

    GetOwner()->SetLocalPosition(glm::vec3{ x, y, m_center.z });
}