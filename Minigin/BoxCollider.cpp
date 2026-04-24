#include "BoxCollider.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Transform.h"

dae::BoxCollider::BoxCollider(GameObject* owner, const glm::vec2& size):
	Component(owner)
	,m_Size(size)
{
}

void dae::BoxCollider::Update()
{
}

void dae::BoxCollider::Render() const
{
	if (m_DrawDebug == false)
	{
		return;
	}

	const glm::vec2 position = GetPosition();
	Renderer::GetInstance().DrawRect(
		position.x,
		position.y,
		m_Size.x,
		m_Size.y
	);

}

bool dae::BoxCollider::IsCollidingWith(const BoxCollider& other) const
{
	const glm::vec2 aPos = GetPosition();
	const glm::vec2 bPos = other.GetPosition();

	const glm::vec2 aSize = GetSize();
	const glm::vec2 bSize = other.GetSize();

	const float aLeft = aPos.x;
	const float aRight = aPos.x + aSize.x;
	const float aTop = aPos.y;
	const float aBottom = aPos.y + aSize.y;

	const float bLeft = bPos.x;
	const float bRight = bPos.x + bSize.x;
	const float bTop = bPos.y;
	const float bBottom = bPos.y + bSize.y;

	bool overlapX = false;
	bool overlapY = false;

	if (aLeft < bRight && aRight > bLeft)
	{
		overlapX = true;
	}
	if (aTop < bBottom && aBottom > bTop)
	{
		overlapY = true;
	}
	if (overlapX && overlapY)
	{
		return true;
	}
	return false;
}

glm::vec2 dae::BoxCollider::GetPosition() const
{
	const glm::vec3 worldPosition = GetOwner()->GetTransform().GetWorldPosition();
	return glm::vec2{ worldPosition.x, worldPosition.y };
}

glm::vec2 dae::BoxCollider::GetSize() const
{
	return m_Size;
}

void dae::BoxCollider::SetSize(const glm::vec2& size)
{
	m_Size = size;
}

void dae::BoxCollider::SetDrawDebug(bool drawDebug)
{
	m_DrawDebug = drawDebug;
}

bool dae::BoxCollider::GetDrawDebug() const
{
	return m_DrawDebug;
}







