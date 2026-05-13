#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	if (m_scenes.empty()) return;
	m_scenes[m_activeSceneIndex]->Update();
}

void dae::SceneManager::Render()
{
	if (m_scenes.empty()) return;
	m_scenes[m_activeSceneIndex]->Render();
}

dae::Scene& dae::SceneManager::CreateScene()
{
	m_scenes.emplace_back(new Scene());
	return *m_scenes.back();
}

void dae::SceneManager::SetActiveScene(size_t index)
{
	if (index >= m_scenes.size())
		return;

	m_activeSceneIndex = index;
}

dae::Scene& dae::SceneManager::GetActiveScene()
{
	return *m_scenes[m_activeSceneIndex];
}

