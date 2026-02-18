#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include "Component.h"
#include <vector>
#include <type_traits>


namespace dae
{
	class Texture2D;
	class GameObject final
	{
		Transform m_transform{};
		std::shared_ptr<Texture2D> m_texture{};
		std::vector<std::unique_ptr<Component>> m_components{};
	public:
		void Update();
		void Render() const;

		//This is to be able to change position of GO
		Transform& GetTransform() { return m_transform; }
		//this is to be able to read position of GO from const methods for example in Render 
		const Transform& GetTransform() const { return m_transform; }

		glm::vec3 GetPosition() const { return m_transform.GetPosition(); }

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);
		//---Componennt interface---
		//Variadic template https://www.geeksforgeeks.org/cpp/variadic-function-templates-c/
		template<typename T, typename... Args> // T -> component that I want to add, Args... -> arguments that I want to pass to the constructor of the component, I use variadic templates because I want to be able to pass any number of arguments to the constructor of the component, and I use perfect forwarding because I want to be able to pass lvalues and rvalues to the constructor of the component, and I want to avoid unnecessary copies of the arguments
		//Pass all parameters to the constructor without copying them,
		void AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
			//std::forward always keep the same category, so for example with renderTexturecomponent if I pass ,,Texture.txt" as lvalue it's lvalue, but if I pass variable path then it's r value
			T* existingComponent = GetComponent<T>();
			if (existingComponent != nullptr)
			{
				return; //I don't want to add multiple components of the same type to the same game object
			}
			
			auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
			m_components.emplace_back(std::move(component)); //I move the unique pointer to the vector, because I want the vector to take ownership of the component
		}

		template<typename T>
		void RemoveComponent(T* component)
		{
			static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
			if (!component) return;
			if (component->GetOwner() != this) return; //I don't want to be able to delete component from other GO
			component->m_markedForRemoval = true;
		}

		template<typename T>
		T* GetComponent()
		{
			static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
			for (auto& component : m_components)
			{
				//for every component I try to cast it to the type T, if cast success I return it
				T* casted = dynamic_cast<T*>(component.get());
				if (casted != nullptr)
				{
					return casted;
				}
			}
			return nullptr;
		}

		template<typename T>
		bool HasComponent() const
		{
			// GetComponent is not const, but I want to be able to call it from a const function
			GameObject* ConstThis = const_cast<GameObject*>(this);
			T* component = ConstThis->GetComponent<T>();

			if (component != nullptr)
			{
				return true;
			}

			return false;
		}

		void CleanupRemovedComponents();


		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
	};


}
