#pragma once
#include "Component.h"
#include <cstddef>
#include <vector>

namespace dae
{
	class TrashcacheComponent final : public Component
	{
	public:
		explicit TrashcacheComponent(GameObject* owner);

		void Update() override {}
		void Render() const override;

	private:
		void RunExercise1() const;
		void RunTransformLocal() const;
		void RunTransformPointer() const;

	private:
		// --- state ---
		mutable int m_Samples1{ 100 };
		mutable bool m_HasResult1{ false };
		mutable std::vector<float> m_Results1{};

		mutable int m_Samples2{ 100 };
		mutable bool m_HasA{ false };
		mutable bool m_HasB{ false };
		mutable std::vector<float> m_ResultsA{};
		mutable std::vector<float> m_ResultsB{};
	};
}