#include "EnemyBoss.h"
#include "BossState.h"
#include <GameActor.h>
#include "EnemyBoss.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "GameActor.h"
#include <BossFormationState.h>
#include <RenderComponent.h>
#include "ProjectilePoolComponent.h"
#include "RenderComponent.h"
#include "SceneManager.h"

dae::EnemyBoss::EnemyBoss(GameObject* owner, ProjectilePoolComponent& projectilePool):Enemy(owner, projectilePool)
{
	m_State = std::make_unique<BossFormationState>();
	m_Health = 2;
}

void dae::EnemyBoss::TakeDamage()
{
	Enemy::TakeDamage();

	if (m_Health == 1)
	{
		auto renderer = GetOwner()->GetComponent<RenderComponent>();
		renderer->SetTexture("boss_purple.png");
	}
}

void dae::EnemyBoss::Update()
{
	if (m_State == nullptr)
	{
		return;
	}

	auto newState = m_State->Update(*this);

	if (newState)
	{
		ChangeState(std::move(newState));
	}

}

void dae::EnemyBoss::ChangeState(std::unique_ptr<BossState> newState)
{
	if (m_State)
	{
		m_State->OnExit(*this);
	}

	m_State = std::move(newState);

	if (m_State)
	{
		m_State->OnEnter(*this);
	}
}

void dae::EnemyBoss::CreateTractorBeam(Scene& scene)
{
	auto beam = std::make_unique<GameObject>();

	beam->SetParent(GetOwner(), false);
	beam->SetLocalPosition(-85.f, 45.f, 0.f);

	auto* render = beam->AddComponent<RenderComponent>("beam.png");
	render->SetSize(195.f, 250.f);
	render->SetActive(false);

	auto* collider = beam->AddComponent<BoxCollider>(glm::vec2{ 195.f, 250.f });
	collider->SetActive(false);
	collider->SetDrawDebug(true);

	m_pTractorBeam = beam.get();

	scene.Add(std::move(beam));
}

void dae::EnemyBoss::EnableTractorBeam(bool enabled)
{
	m_BeamEnabled = enabled;

	if (!m_pTractorBeam)
		return;

	if (auto* render = m_pTractorBeam->GetComponent<RenderComponent>())
		render->SetActive(enabled);

	if (auto* collider = m_pTractorBeam->GetComponent<BoxCollider>())
		collider->SetActive(enabled);
}