#pragma once
#include "Event.h"

namespace dae
{
	class GameActor;
	class Observer
	{
	public:
		virtual ~Observer() = default;

		virtual void Notify(dae::Event event) = 0;
	};
}