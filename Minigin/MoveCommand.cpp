#include "MoveCommand.h"
#include "MovementComponent.h"
#include "GameTime.h"
#include "GameObject.h"

dae::MoveCommand::MoveCommand(const glm::vec3& direction, GameObject* target)
	:m_Direction(direction),
	m_pTarget{ target },
	m_Speed(100.f)
{
}

void dae::MoveCommand::Execute()
{
	if (m_pTarget == nullptr)
		return;
	const float dt = GameTime::GetIntance().GetDeltaTime();
	auto position = m_pTarget->GetLocalPosition();
	position += m_Direction * m_Speed * dt;
	m_pTarget->SetLocalPosition(position);
}

