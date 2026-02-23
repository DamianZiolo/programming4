#include "Transform.h"

void dae::Transform::SetLocalPosition(const float x, const float y, const float z)
{
	m_localPosition.x = x;
	m_localPosition.y = y;
	m_localPosition.z = z;
}

void dae::Transform::SetLocalPosition(const glm::vec3& position)
{ 
	m_localPosition = position;
}

void dae::Transform::SetWorldPosition(float x, float y, float z)
{
	m_worldPosition.x = x;
	m_worldPosition.y = y;
	m_worldPosition.z = z;
}

void dae::Transform::SetWorldPosition(const glm::vec3& position)
{
	m_worldPosition = position;
}
