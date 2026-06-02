#pragma once

#include "Command.h"

namespace dae
{
	class ToggleMuteCommand final : public Command
	{
	public:
		ToggleMuteCommand() = default;
		~ToggleMuteCommand() override = default;

		void Execute() override;
	};
}