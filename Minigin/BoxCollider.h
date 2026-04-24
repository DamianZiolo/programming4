#pragma once
#include <glm/glm.hpp>
#include "Component.h"

namespace dae 
{
	class BoxCollider final: public Component
	{
	public:
		BoxCollider(GameObject* owner, const glm::vec2& size);

		void Update() override;
		void Render() const override;

		bool IsCollidingWith(const BoxCollider& other) const;

		glm::vec2 GetPosition() const;
		glm::vec2 GetSize() const;

		void SetSize(const glm::vec2& size);
		void SetDrawDebug(bool drawDebug);
		bool GetDrawDebug() const;

	private:
		glm::vec2 m_Size{};
		bool m_DrawDebug{ false };
	};

}