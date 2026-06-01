#pragma once

#include "Component.h"

namespace dae
{
	class TractorBeamComponent final : public Component
	{
	public:
		explicit TractorBeamComponent(GameObject* owner)
			: Component(owner){}

		void Update() override {}
	};
}