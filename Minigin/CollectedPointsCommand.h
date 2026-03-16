#pragma once
#include "Command.h"

namespace dae
{
	class GameObject;

	class CollectedPointsCommand : public Command
	{
	public:
		explicit CollectedPointsCommand(GameObject* player, int points);

		void Execute() override;

	private:
		GameObject* m_pTarget{};
		int m_Points{};
	};
}