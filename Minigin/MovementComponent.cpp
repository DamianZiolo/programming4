#include "MovementComponent.h"
#include "GameObject.h"
#include "GameTime.h"

dae::MovementComponent::MovementComponent(GameObject* owner, float speed):
	Component(owner),
	m_Speed{speed}
{
}

void dae::MovementComponent::Update()
{
	//In future
}

void dae::MovementComponent::Move(const glm::vec3& direction)
{
	auto* owner = GetOwner();
	if (!owner)
		return;

	const float dt = GameTime::GetIntance().GetDeltaTime();

	auto position = owner->GetLocalPosition();
	position += direction * m_Speed * dt;
	owner->SetLocalPosition(position);
}