#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	UpdateWorldTransform();
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

void dae::GameObject::UpdateWorldTransform()
{
	if (!m_transform.IsDirty())
	{
		return;
	}
	
	//if object has a parent, its world position is the sum of its local position and its parent's world position, otherwise its world position is the same as its local position
	if (m_parent)
	{
		m_parent->UpdateWorldTransform(); //first update position of parent, because I need it to calculate the world position of this object

		const glm::vec3 parentWorldPos = m_parent->GetWorldPosition();
		const glm::vec3 newWorldPos = parentWorldPos + m_transform.GetLocalPosition();
		m_transform.SetWorldPosition(newWorldPos);
	}
	else 
	{
		m_transform.SetWorldPosition(m_transform.GetLocalPosition());
	}

	m_transform.ClearDirty();

	for (auto* child : m_children)
	{
		if (child)
			child->SetDirty(); 
	}
}

void dae::GameObject::SetDirty()
{
	m_transform.SetDirty();
}

void dae::GameObject::SetParent(GameObject* newParent, bool keepWorld)
{
	//If the new parent is the same as the current parent, do nothing
	if (newParent == this) return;
	//If the new parent is a descendant of this game object, do nothing, because it would create a cycle in the scene graph, and we don't want that, because it would lead to issues with updating and rendering, and also because it's not necessary to have a cycle in the scene graph, we can just set the parent to nullptr if we want to detach the game object from its current parent
	if (newParent && newParent->IsDescendantOf(this)) return;
	if(keepWorld)
	{
		glm::vec3 worldPos = GetWorldPosition();
		if (m_parent)
		{
			m_parent->RemoveChild(this);
		}
		m_parent = newParent;
		if (m_parent)
		{
			m_parent->AddChild(this);
			SetWorldPosition(worldPos);
		}
	}
	else
	{
		if (m_parent)
		{
			m_parent->RemoveChild(this);
		}
		m_parent = newParent;
		if (m_parent) 
		{
			m_parent->AddChild(this);
		}
		SetDirty();
	}

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