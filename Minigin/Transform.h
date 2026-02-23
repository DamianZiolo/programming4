#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		const glm::vec3& GetWorldPosition() const { return m_worldPosition; }
		const glm::vec3& GetLocalPosition() const { return m_localPosition; }
		void SetLocalPosition(float x, float y, float z = 0);
		void SetLocalPosition(const glm::vec3& position);
		void SetWorldPosition(float x, float y, float z = 0);
		void SetWorldPosition(const glm::vec3& position);
	private:
		glm::vec3 m_worldPosition;
		glm::vec3 m_localPosition;
		bool m_IsDirty{ true };
	};
}
