#pragma once
#include <glm/vec3.hpp>
#include "Command.h"

namespace dae
{
	class MovementComponent;
	
	class MoveCommand final : public Command
	{
	public:
		MoveCommand(MovementComponent* movementComponent, const glm::vec3& direction);

		void Execute() override;

	private:
		MovementComponent* m_pMovementComponent{};
		glm::vec3 m_Direction{};

	};

}