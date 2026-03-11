#include "MoveCommand.h"
#include "MovementComponent.h"

dae::MoveCommand::MoveCommand(MovementComponent* movementComponent, const glm::vec3& direction)
	: m_pMovementComponent(movementComponent)
	, m_Direction(direction)
{
}

void dae::MoveCommand::Execute()
{
	if (!m_pMovementComponent)
		return;

	m_pMovementComponent->Move(m_Direction);
}