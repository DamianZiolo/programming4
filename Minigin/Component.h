#pragma once
#include <cassert>

namespace dae
{
	class GameObject;

	class Component
	{
	public: 
		virtual ~Component() = default;

		virtual void Update() = 0; //pure virtual function;
		virtual void Render() const {}
		GameObject* GetOwner() const { return m_Owner; }
		bool IsActive() const { return m_IsActive; }
		void SetActive(bool active) { m_IsActive = active; }

		//I delete it because component is not meant to be copied or moved, because it has a pointer to its owner, and if we copy or move it, we would have multiple components pointing to the same owner, which can lead to issues with ownership and memory management, and also because it's not necessary to copy or move components, we can just create new ones and add them to the game object
		Component(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator=(const Component&) = delete;
		Component& operator=(Component&&) = delete;

	protected:
		explicit Component(GameObject* owner) : m_Owner(owner)
		{
			assert(owner && "Component must have an owner GameObject");
		}

	private:
		GameObject* m_Owner{};
		bool m_markedForRemoval{ false };
		bool m_IsActive{ true };
		friend class GameObject; //I make GameObject a friend class so it can access the private members of Component, because I want to be able to mark components for removal from the game object, and I don't want to expose that functionality to other classes, because it's not necessary for them to know about it, and it can lead to issues with encapsulation and separation of concerns
	};

}