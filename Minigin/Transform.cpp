#include "Transform.h"

void dae::Transform::SetLocalPosition(const float x, const float y, const float z)
{
	SetLocalPosition({ x, y, z });
}

void dae::Transform::SetLocalPosition(const glm::vec3& position)
{ 
	m_localPosition = position;
	//After we change the local position, we need to mark the transform as dirty, because the world position will change, and we need to recalculate it in the next update
	SetDirty();
}

void dae::Transform::SetWorldPosition(float x, float y, float z) const
{
	SetWorldPosition({ x, y, z });
}

void dae::Transform::SetWorldPosition(const glm::vec3& position) const
{
	m_worldPosition = position;
}
