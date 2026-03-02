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
	SetDirty();
	m_transform.SetLocalPosition(local);
}

glm::vec3 dae::GameObject::GetWorldPosition()
{
	UpdateWorldTransform();
	return m_transform.GetWorldPosition();
}

void dae::GameObject::SetWorldPosition(const glm::vec3& world)
{
	m_transform.SetWorldPosition(world);
}

void dae::GameObject::UpdateWorldTransform()
{
	if (m_transform.IsDirty() )
	{
		if (m_parent == nullptr)
			m_transform.SetWorldPosition(m_transform.GetLocalPosition() );
		else
			m_transform.SetWorldPosition(m_parent->GetWorldPosition() + m_transform.GetLocalPosition() );
	}
	m_transform.ClearDirty();
}

void dae::GameObject::SetDirty()
{
	m_transform.SetDirty();
	for(GameObject* child : m_children)
	{
		child->SetDirty();
	}
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	if (IsDescendantOf(parent) || parent == this || m_parent == parent)
		return;
	if (parent == nullptr)
		SetLocalPosition(GetWorldPosition());
	else
	{
		if (keepWorldPosition)
			SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
		else
			SetDirty();
	}
	if (m_parent) m_parent->RemoveChild(this);
	m_parent = parent;
	if (m_parent) m_parent->AddChild(this);
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