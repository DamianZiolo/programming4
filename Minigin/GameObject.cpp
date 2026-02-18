#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

void dae::GameObject::CleanupRemovedComponents()
{
	//if Lambda returns true, the element will be removed from the vector
	std::erase_if(m_components, [](const std::unique_ptr<Component>& c)
		{
			return c->m_markedForRemoval;
		});
}

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(){

	for (const auto& c : m_components)
	{
		if (c->IsActive())
			c->Update();
	}

}

void dae::GameObject::Render() const
{
	//old render, I will delete it later after adding Render component
	if (m_texture)
	{
		const auto& pos = m_transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	}

	for (const auto& c : m_components)
	{
		if (c->IsActive())
			c->Render();
	}

}

void dae::GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
