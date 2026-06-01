#pragma once
#include "EnemyFlyState.h"
#include "GameSettings.h"
namespace dae
{
	class EnemyFlyBombingState final : public EnemyFlyState
	{
	public:
		EnemyFlyBombingState() = default;
		~EnemyFlyBombingState() override = default;

		void OnEnter(EnemyFly& enemy) override;
		std::unique_ptr<EnemyFlyState> Update(EnemyFly& enemy) override;

	private:
		float m_DownSpeed{ 180.f };
		float m_HorizontalSpeed{};
		static constexpr float shootLine = GameSettings::ScreenHeight - 300.f;

		bool m_HasShot{ false };
	};
}