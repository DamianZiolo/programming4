#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{

	for (const auto& c : m_components)
	{
		if (c->IsActive())
			c->Update();
	}

}

void dae::GameObject::Render() const
{
	for (const auto& c : m_components)
	{
		if (c->IsActive())
			c->Render();
	}

}

void dae::GameObject::SetLocalPosition(const glm::vec3& local)
{
	m_transform.SetLocalPosition(local);
}

void dae::GameObject::SetWorldPosition(const glm::vec3& world)
{
	m_transform.SetWorldPosition(world);
}

void dae::GameObject::SetDirty()
{
	//TODO: implement dirty flag
}

void dae::GameObject::SetParent(GameObject* newParent, bool keepWorld)
{
	(void)newParent;
	(void)keepWorld;
	/*if (newParent == this) return;
	if (newParent && newParent->IsDescendantOf(this)) return;
	glm::vec3 worldPos = m_transform.GetLocalPosition(); 
	if (m_parent)
	{
		m_parent->RemoveChild(this);
	}

	m_parent = newParent;

	if (m_parent)
	{
		m_parent->AddChild(this);
	}*/

	//TODO still need to implement it 
}

bool dae::GameObject::IsDescendantOf(const GameObject* potentialAncestor) const
{
	const GameObject* p = m_parent;
	while (p)
	{
		if (p == potentialAncestor) return true;
		p = p->m_parent;
	}
	return false;
}

void dae::GameObject::CleanupRemovedComponents()
{
	//if Lambda returns true, the element will be removed from the vector
	std::erase_if(m_components, [](const std::unique_ptr<Component>& c)
		{
			return c->m_markedForRemoval;
		});
}

void dae::GameObject::AddChild(GameObject* child)
{
	m_children.push_back(child);
}

void dae::GameObject::RemoveChild(GameObject* child)
{
	std::erase(m_children, child);
}