#include <LevelManagerComponent.h>

#include "Scene.h"
#include "GameObject.h"
#include "FleetComponent.h"
#include "RenderComponent.h"
#include "BoxCollider.h"
#include "Enemy.h"
#include "EnemyBoss.h"
#include "EnemyFly.h"
#include "EnemyButterfly.h"
#include "ProjectilePoolComponent.h"
#include "ResourceManager.h"
#include "EnemyBoss.h"
#include "GameSettings.h"
#include <fstream>
#include <sstream>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
//I hate this very much wrrr
static void Log(const std::string& message)
{
	OutputDebugStringA((message + "\n").c_str());
}

dae::LevelManagerComponent::LevelManagerComponent(
	GameObject* owner,
	Scene& scene,
	ProjectilePoolComponent& projectilePool)
	: Component(owner)
	, m_Scene{ scene }
	, m_ProjectilePool{ projectilePool }
{
	Log("[LevelManager] Created");
	LoadCurrentLevel();
}

void dae::LevelManagerComponent::Update()
{
	if (!m_pCurrentFleet)
		return;

	if (IsCurrentLevelCleared())
	{
		LoadNextLevel();
	}
}

void dae::LevelManagerComponent::SendRandomBossAttack(BossAttackType attackType)
{
	if (!m_pCurrentFleet)
		return;

	std::vector<EnemyBoss*> availableBosses{};

	for (auto* slot : m_pCurrentFleet->GetChildren())
	{
		if (!slot)
			continue;

		for (auto* child : slot->GetChildren())
		{
			if (!child)
				continue;

			auto* boss = child->GetComponent<EnemyBoss>();

			if (boss && !boss->IsAttacking())
			{
				availableBosses.push_back(boss);
			}
		}
	}

	if (availableBosses.empty())
		return;

	const int randomIndex =
		rand() % static_cast<int>(availableBosses.size());

	availableBosses[randomIndex]->RequestAttack(attackType);
}

void dae::LevelManagerComponent::LoadCurrentLevel()
{
	if (m_CurrentLevelIndex >= static_cast<int>(m_LevelFiles.size()))
	{
		Log("[LevelManager] No more levels");
		return;
	}

	const auto& filename = m_LevelFiles[m_CurrentLevelIndex];

	Log("[LevelManager] Loading: " + filename);

	const auto layout = LoadLevelLayout(filename);

	if (layout.empty())
	{
		Log("[LevelManager] ERROR: Layout is empty");
		return;
	}

	CreateFleetFromLayout(layout);

	if (!m_pCurrentFleet)
	{
		Log("[LevelManager] ERROR: Could not create fleet from: " + filename);
		return;
	}

	Log("[LevelManager] Fleet created");
}

void dae::LevelManagerComponent::LoadNextLevel()
{
	ClearCurrentLevel();

	++m_CurrentLevelIndex;

	if (m_CurrentLevelIndex >= static_cast<int>(m_LevelFiles.size()))
	{
		m_pCurrentFleet = nullptr;
		m_AllLevelsCompleted = true;
		Log("[LevelManager] Finished all levels");
		return;
	}

	LoadCurrentLevel();
}

std::vector<std::string> dae::LevelManagerComponent::LoadLevelLayout(
	const std::string& filename) const
{
	std::vector<std::string> layout{};

	const auto fullPath =
		ResourceManager::GetInstance().GetDataPath() / filename;

	Log("[LevelManager] Full path: " + fullPath.string());

	std::ifstream file{ fullPath };

	if (!file.is_open())
	{
		Log("[LevelManager] ERROR: Could not open level file");
		return layout;
	}

	std::string line{};

	while (std::getline(file, line))
	{
		if (!line.empty() && line.back() == '\r')
			line.pop_back();

		if (!line.empty())
		{
			layout.push_back(line);
			Log("[LevelManager] Row: " + line);
		}
	}

	std::ostringstream ss;
	ss << "[LevelManager] Loaded rows: " << layout.size();
	Log(ss.str());

	return layout;
}

void dae::LevelManagerComponent::CreateFleetFromLayout(
	const std::vector<std::string>& layout)
{
	const int rows = static_cast<int>(layout.size());

	if (rows == 0)
	{
		Log("[LevelManager] ERROR: Layout has 0 rows");
		return;
	}

	const int cols = static_cast<int>(layout[0].size());

	if (cols == 0)
	{
		Log("[LevelManager] ERROR: Layout has 0 cols");
		return;
	}

	const float spacingX = 60.f;
	const float spacingY = 45.f;

	const float formationWidth = (cols - 1) * spacingX;

	const float startX =
		(dae::GameSettings::ScreenWidth - formationWidth) * 0.5f;

	const float startY = 80.f;

	auto fleet = std::make_unique<GameObject>();
	fleet->SetLocalPosition(glm::vec3{ startX, startY, 0.f });

	auto* fleetRaw = fleet.get();

	auto* fleetComponent = fleet->AddComponent<FleetComponent>(
		rows,
		cols,
		spacingX,
		spacingY);

	m_pCurrentFleet = fleetRaw;
	m_Scene.Add(std::move(fleet));

	int enemyCount{};

	for (int row = 0; row < rows; ++row)
	{
		for (int col = 0; col < cols; ++col)
		{
			if (col >= static_cast<int>(layout[row].size()))
				continue;

			auto slot = std::make_unique<GameObject>();
			auto* slotRaw = slot.get();

			m_Scene.Add(std::move(slot));

			slotRaw->SetParent(fleetRaw, false);
			slotRaw->SetLocalPosition(col * spacingX, row * spacingY, 0.f);

			fleetComponent->SetSlot(row, col, slotRaw);

			const char enemyType = layout[row][col];

			std::unique_ptr<GameObject> enemy{};

			switch (enemyType)
			{
			case 'B':
			{
				enemy = std::make_unique<GameObject>();

				auto* render = enemy->AddComponent<RenderComponent>("Boss.png");
				render->SetSize(50.f, 50.f);

				auto* collider = enemy->AddComponent<BoxCollider>(glm::vec2{ 50.f, 50.f });
				collider->SetDrawDebug(true);

				auto* boss = enemy->AddComponent<EnemyBoss>(m_ProjectilePool);
				boss->CreateTractorBeam(m_Scene);

				Log("[LevelManager] Spawn Boss");
				break;
			}

			case 'F':
			{
				enemy = std::make_unique<GameObject>();

				auto* render = enemy->AddComponent<RenderComponent>("Enemy1.png");
				render->SetSize(30.f, 30.f);

				auto* collider = enemy->AddComponent<BoxCollider>(glm::vec2{ 30.f, 30.f });
				collider->SetDrawDebug(true);

				enemy->AddComponent<EnemyFly>(m_ProjectilePool);

				Log("[LevelManager] Spawn Fly");
				break;
			}

			case 'T':
			{
				enemy = std::make_unique<GameObject>();

				auto* render = enemy->AddComponent<RenderComponent>("Enemy2.png");
				render->SetSize(30.f, 30.f);

				auto* collider = enemy->AddComponent<BoxCollider>(glm::vec2{ 30.f, 30.f });
				collider->SetDrawDebug(true);

				enemy->AddComponent<EnemyButterfly>(m_ProjectilePool);

				Log("[LevelManager] Spawn Butterfly");
				break;
			}

			case '.':
				break;

			default:
			{
				std::string msg = "[LevelManager] Unknown char: ";
				msg += enemyType;
				Log(msg);
				break;
			}
			}

			if (enemy)
			{
				auto* enemyRaw = enemy.get();

				m_Scene.Add(std::move(enemy));

				enemyRaw->SetParent(slotRaw, false);

				if (auto* enemyComponent = enemyRaw->GetComponent<Enemy>())
				{
					enemyComponent->SetSlot(slotRaw);
				}

				++enemyCount;
			}
		}
	}

	std::ostringstream ss;
	ss << "[LevelManager] Total enemies spawned: " << enemyCount;
	Log(ss.str());
}

void dae::LevelManagerComponent::ClearCurrentLevel()
{
	if (m_pCurrentFleet)
	{
		Log("[LevelManager] Clearing current level");

		m_pCurrentFleet->MarkForRemoval();
		m_pCurrentFleet = nullptr;
	}
}

bool dae::LevelManagerComponent::IsCurrentLevelCleared() const
{
	const auto& objects = m_Scene.GetObjects();

	for (const auto& object : objects)
	{
		if (!object)
			continue;

		if (object->IsMarkedForRemoval())
			continue;

		if (object->GetComponent<Enemy>())
			return false;
	}

	return true;
}