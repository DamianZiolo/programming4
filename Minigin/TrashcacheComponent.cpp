#include "TrashcacheComponent.h"

#include <imgui.h>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <memory>
#include <vector>

namespace dae
{
	TrashcacheComponent::TrashcacheComponent(GameObject* owner)
		: Component(owner)
	{
		m_Results1.reserve(11);
		m_ResultsA.reserve(11);
		m_ResultsB.reserve(11);
	}

	void TrashcacheComponent::Render() const
	{
		// ---------------- Exercise 1 ----------------
		ImGui::Begin("Exercise 1");
		ImGui::Text("Pick number of samples");
		ImGui::InputInt("Samples", &m_Samples1);

		if (ImGui::Button("Trash the cache"))
		{
			RunExercise1();
		}

		if (m_HasResult1 && !m_Results1.empty())
		{
			ImGui::PlotLines(
				"Execution Time (us)",
				m_Results1.data(),
				static_cast<int>(m_Results1.size()),
				0,
				nullptr,
				0.0f,
				*std::max_element(m_Results1.begin(), m_Results1.end()),
				ImVec2(0, 200)
			);
		}
		ImGui::End();

		// ---------------- Exercise 2 ----------------
		ImGui::Begin("Exercise 2");
		ImGui::Text("Pick number of samples");
		ImGui::InputInt("Samples", &m_Samples2);

		if (ImGui::Button("Trash cache - Transform local"))
		{
			RunTransformLocal();
		}

		if (ImGui::Button("Trash cache - Transform pointer"))
		{
			RunTransformPointer();
		}

		// --- Plot A (red) ---
		if (m_HasA && !m_ResultsA.empty())
		{
			ImGui::Text("Transform local");
			ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(1, 0, 0, 1));

			ImGui::PlotLines(
				"##PlotA",
				m_ResultsA.data(),
				(int)m_ResultsA.size(),
				0,
				nullptr,
				0.0f,
				*std::max_element(m_ResultsA.begin(), m_ResultsA.end()),
				ImVec2(0, 200)
			);

			ImGui::PopStyleColor();
		}

		// --- Plot B (green) ---
		if (m_HasB && !m_ResultsB.empty())
		{
			ImGui::Text("Transform pointer");
			ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0, 1, 0, 1));

			ImGui::PlotLines(
				"##PlotB",
				m_ResultsB.data(),
				(int)m_ResultsB.size(),
				0,
				nullptr,
				0.0f,
				*std::max_element(m_ResultsB.begin(), m_ResultsB.end()),
				ImVec2(0, 200)
			);

			ImGui::PopStyleColor();
		}

		ImGui::End();
	}

	// ====================== CALCULATIONS ======================

	void dae::TrashcacheComponent::RunExercise1() const
	{
		if (m_Samples1 < 3) m_Samples1 = 3;
		const size_t repeats = static_cast<size_t>(m_Samples1);

		const size_t arraySize = 100000; // wróć do sensownego rozmiaru na start
		// (100'000'000 to 400MB i może mieszać przez paging)

		m_Results1.clear();
		m_Results1.reserve(11);

		auto arr = std::make_unique<int[]>(arraySize);
		for (size_t i = 0; i < arraySize; ++i)
			arr[i] = static_cast<int>(i);

		for (size_t step = 1; step <= 1024; step *= 2)
		{
			std::vector<int64_t> timeArray;
			timeArray.reserve(repeats);

			for (size_t j = 0; j < repeats; ++j)
			{
				auto t0 = std::chrono::high_resolution_clock::now();

				for (size_t i = 0; i < arraySize; i += step)
				{
					arr[i] *= 2;
				}

				auto t1 = std::chrono::high_resolution_clock::now();
				auto timeNs = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
				timeArray.push_back((int64_t)timeNs);
			}

			std::sort(timeArray.begin(), timeArray.end());

			int64_t sum = 0;
			for (size_t k = 1; k < timeArray.size() - 1; ++k)
				sum += timeArray[k];

			double avgNs = (double)sum / (double)(timeArray.size() - 2);

			// jeśli chcesz dalej rysować w "us", przelicz:
			double avgUs = avgNs / 1000.0;
			m_Results1.push_back((float)avgUs);
		}

		m_HasResult1 = true;
	}

	void TrashcacheComponent::RunTransformLocal() const
	{
		m_ResultsA.clear();
		m_ResultsA.reserve(11);

		if (m_Samples2 < 3) m_Samples2 = 3;
		const size_t repeats = static_cast<size_t>(m_Samples2);

		struct Transform { float matrix[16]; };
		struct GameObject3DLike
		{
			Transform local;
			int id;
		};

		const size_t arraySize = 1000000;
		auto arr = std::make_unique<GameObject3DLike[]>(arraySize);

		for (size_t i = 0; i < arraySize; ++i)
			arr[i].id = static_cast<int>(i);

		for (size_t step = 1; step <= 1024; step *= 2)
		{
			std::vector<int64_t> timeArray;
			timeArray.reserve(repeats);

			for (size_t j = 0; j < repeats; ++j)
			{
				auto t0 = std::chrono::high_resolution_clock::now();

				for (size_t i = 0; i < arraySize; i += step)
					arr[i].id *= 2;

				auto t1 = std::chrono::high_resolution_clock::now();
				auto time = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
				timeArray.push_back((int64_t)time);
			}

			std::sort(timeArray.begin(), timeArray.end());

			int64_t sum = 0;
			for (size_t k = 1; k < timeArray.size() - 1; ++k)
				sum += timeArray[k];

			double avg = (double)sum / (double)(timeArray.size() - 2);
			m_ResultsA.push_back((float)avg);
		}

		m_HasA = true;
	}

	void TrashcacheComponent::RunTransformPointer() const
	{
		m_ResultsB.clear();
		m_ResultsB.reserve(11);

		if (m_Samples2 < 3) m_Samples2 = 3;
		const size_t repeats = static_cast<size_t>(m_Samples2);

		struct Transform { float matrix[16]; };
		struct GameObject3DLike
		{
			std::unique_ptr<Transform> local;
			int id;
		};

		const size_t arraySize = 100'000;
		auto arr = std::make_unique<GameObject3DLike[]>(arraySize);

		for (size_t i = 0; i < arraySize; ++i)
		{
			arr[i].id = static_cast<int>(i);
			arr[i].local = std::make_unique<Transform>();
			arr[i].local->matrix[0] = 0.0f;
		}

		for (size_t step = 1; step <= 1024; step *= 2)
		{
			std::vector<int64_t> timeArray;
			timeArray.reserve(repeats);

			for (size_t j = 0; j < repeats; ++j)
			{
				auto t0 = std::chrono::high_resolution_clock::now();

				for (size_t i = 0; i < arraySize; i += step)
				{
					arr[i].id *= 2;
					arr[i].local->matrix[0] += 1.0f;
				}

				auto t1 = std::chrono::high_resolution_clock::now();
				auto time = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
				timeArray.push_back((int64_t)time);
			}

			std::sort(timeArray.begin(), timeArray.end());

			int64_t sum = 0;
			for (size_t k = 1; k < timeArray.size() - 1; ++k)
				sum += timeArray[k];

			double avg = (double)sum / (double)(timeArray.size() - 2);
			m_ResultsB.push_back((float)avg);
		}

		m_HasB = true;
	}
}