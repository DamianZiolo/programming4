#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "Renderer.h"
#include "GameObject.h"

namespace dae
{
	dae::RenderComponent::RenderComponent(GameObject* owner, const std::string& filename) : dae::Component(owner)
	{
		SetTexture(filename);
		UpdatePosition();
	}

	void RenderComponent::UpdatePosition()
	{
		m_Transform = &GetOwner()->GetTransform();
	}

	void RenderComponent::Render() const
	{
		if (m_Texture && m_Transform)
		{
			if (m_Height > 0.f && m_Width > 0.f)
			{
				const auto& pos = GetOwner()->GetWorldPosition();
				Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, m_Width, m_Height);
				return;
			}
			const auto& pos = GetOwner()->GetWorldPosition();
			Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
		}
		
	}

	void dae::RenderComponent::SetTexture(const std::string& filename)
	{
		m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
	}
}