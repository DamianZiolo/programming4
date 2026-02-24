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

		void SetWorldPosition(float x, float y, float z = 0) const;
		void SetWorldPosition(const glm::vec3& position) const;

		bool IsDirty() const { return m_IsDirty; }
		void SetDirty() const { m_IsDirty = true; }
		void ClearDirty() const { m_IsDirty = false; }
	private:
		mutable glm::vec3 m_worldPosition{ 0.f, 0.f, 0.f };
		glm::vec3 m_localPosition{ 0.f, 0.f, 0.f };
		mutable bool m_IsDirty{ true };
	};
}
