#pragma once
#include "Command.h"

namespace dae {

	class GameObject;
	class ShotCommand : public Command
	{
	public:
		explicit ShotCommand(GameObject* player);
		void Execute() override;
	private:
		GameObject* m_pTarget{};
	};

}