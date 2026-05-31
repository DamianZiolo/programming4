#include <algorithm>
#include <cassert>

#include "Scene.h"
#include "GameObject.h"

using namespace dae;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");

	if (m_IsUpdating)
	{
		m_PendingObjects.emplace_back(std::move(object));
		return;
	}

	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(GameObject& object)
{
	object.MarkForRemoval();
}

void Scene::RemoveAll()
{
	m_objects.clear();
	m_PendingObjects.clear();
}

void Scene::Update()
{
	m_IsUpdating = true;

	for (auto& object : m_objects)
	{
		if (object && !object->IsMarkedForRemoval())
		{
			object->Update();
		}
	}

	m_IsUpdating = false;

	if (!m_PendingObjects.empty())
	{
		for (auto& object : m_PendingObjects)
		{
			m_objects.emplace_back(std::move(object));
		}

		m_PendingObjects.clear();
	}

	std::erase_if(m_objects, [](const std::unique_ptr<GameObject>& obj)
		{
			return !obj || obj->IsMarkedForRemoval();
		});
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		if (object && !object->IsMarkedForRemoval())
		{
			object->Render();
		}
	}
}