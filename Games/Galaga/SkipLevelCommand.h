#pragma once

#include "Command.h"

namespace dae
{
	class LevelManagerComponent;

	class SkipLevelCommand final : public Command
	{
	public:
		explicit SkipLevelCommand(LevelManagerComponent* levelManager);

		void Execute() override;

	private:
		LevelManagerComponent* m_pLevelManager{};
	};
}