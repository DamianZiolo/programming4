#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include "Component.h"
#include <vector>


namespace dae
{
	class Texture2D;
	class GameObject 
	{
		Transform m_transform{};
		std::shared_ptr<Texture2D> m_texture{};
	public:
		virtual void Update();
		virtual void Render() const;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		std::vector<std::unique_ptr<Component>> m_components{};

		//---Componennt interface---
		//Variadic template https://www.geeksforgeeks.org/cpp/variadic-function-templates-c/
		template<typename T, typename... Args> // T -> component that I want to add, Args... -> arguments that I want to pass to the constructor of the component, I use variadic templates because I want to be able to pass any number of arguments to the constructor of the component, and I use perfect forwarding because I want to be able to pass lvalues and rvalues to the constructor of the component, and I want to avoid unnecessary copies of the arguments
		//Pass all parameters to the constructor without copying them,
		void AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
			//std::forward always keep the same category, so for example with renderTexturecomponent if I pass ,,Texture.txt" as lvalue it's lvalue, but if I pass variable path then it's r value
			auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
			m_components.emplace_back(std::move(component)); //I move the unique pointer to the vector, because I want the vector to take ownership of the component
		}

		template<typename T>
		void RemoveComponent(T* component)
		{
			static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
			if (!component) return;
			if (component->GetOwner() != this) return; // albo assert
			component->m_markedForRemoval = true;
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
