#pragma once
#include <glm/vec3.hpp>
#include "Command.h"

namespace dae
{
	class GameObject;
	
	class MoveCommand final : public Command
	{
	public:
		MoveCommand(const glm::vec3& direction, GameObject* target);

		void Execute() override;
		float GetSpeed() const { return m_Speed; }
		void SetSpeed(float speed) { m_Speed = speed; }

	private:
		float m_Speed{};
		glm::vec3 m_Direction{};
		GameObject* m_pTarget{};

	};

}