#include "Event.h"

namespace dae
{
	class GameActor;
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(Event event, GameActor* actor) = 0;

	};
}