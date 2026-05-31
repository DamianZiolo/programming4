#pragma once

#include "Component.h"

#include <memory>
#include <string>
#include <vector>

namespace dae
{
	class Scene;
	class GameObject;
	class ProjectilePoolComponent;

	class LevelManagerComponent final : public Component
	{
	public:
		LevelManagerComponent(
			GameObject* owner,
			Scene& scene,
			ProjectilePoolComponent& projectilePool);

		void Update() override;
		bool AreAllLevelsCompleted() const { return m_AllLevelsCompleted; }
	private:
		void LoadCurrentLevel();
		void LoadNextLevel();

		std::vector<std::string> LoadLevelLayout(const std::string& filename) const;

		void CreateFleetFromLayout(const std::vector<std::string>& layout);

		void ClearCurrentLevel();
		bool IsCurrentLevelCleared() const;

		Scene& m_Scene;
		ProjectilePoolComponent& m_ProjectilePool;

		std::vector<std::string> m_LevelFiles{
			"Levels/level1.txt",
			"Levels/level2.txt",
			"Levels/level3.txt"
		};

		int m_CurrentLevelIndex{};
		GameObject* m_pCurrentFleet{};
		bool m_AllLevelsCompleted{ false };
	};
}