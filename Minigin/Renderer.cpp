#include <stdexcept>
#include <cstring>
#include <iostream>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <chrono>

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;

	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

#if defined(__EMSCRIPTEN__)
	m_renderer = SDL_CreateRenderer(window, nullptr);
#else
	m_renderer = SDL_CreateRenderer(window, nullptr);
#endif

	if (m_renderer == nullptr)
	{
		std::cout << "Failed to create the renderer: " << SDL_GetError() << "\n";
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
#if __EMSCRIPTEN__
	// For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
	// You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
	io.IniFilename = NULL;
#endif

	ImGui_ImplSDL3_InitForSDLRenderer(window, m_renderer);
	ImGui_ImplSDLRenderer3_Init(m_renderer);

}

void dae::Renderer::Render() const
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	static std::vector<float> results;
	static std::vector<float> results2;
	static bool hasResult1 = false;
	ImGui::Begin("Exercise 1");
	ImGui::Text("Pick number of samples");\
	static int samples = 100;
	ImGui::InputInt("Samples", &samples);
	if (ImGui::Button("Trash the cache"))
	{
		if (samples < 1) samples = 1;                       
		const size_t N = static_cast<size_t>(samples);

		results.clear();
		results.reserve(11);                                 

		auto arr = std::make_unique<int[]>(N);
		for (size_t i = 0; i < N; ++i)
			arr[i] = static_cast<int>(i);

		for (size_t step = 1; step <= 1024; step *= 2)
		{
			auto t0 = std::chrono::high_resolution_clock::now();

			for (size_t i = 0; i < N; i += step)            
				arr[i] *= 2;

			auto t1 = std::chrono::high_resolution_clock::now();

			auto time = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
			results.push_back(static_cast<float>(time));
		}

		hasResult1 = true;
	}
	if (hasResult1 && !results.empty())
	{
		ImGui::PlotLines(
			"Execution Time (us)",
			results.data(),
			static_cast<int>(results.size()),
			0,
			nullptr,
			0.0f,
			*std::max_element(results.begin(), results.end()),
			ImVec2(0, 200)
		);
	}
	ImGui::End();

	ImGui::Begin("Exercise 2");

	ImGui::Text("Pick number of samples");
	static int samples2 = 100;
	ImGui::InputInt("Samples", &samples2);

	static std::vector<float> resultsA;
	static std::vector<float> resultsB;
	static bool hasA = false;
	static bool hasB = false;

	if (ImGui::Button("Trash cache - Transform local"))
	{
		resultsA.clear();
		resultsA.reserve(11);

		if (samples2 < 1) samples2 = 1;
		const size_t N = static_cast<size_t>(samples2);

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
			timeArray.reserve(N);

			for (size_t j = 0; j < N; ++j)
			{
				auto t0 = std::chrono::high_resolution_clock::now();

				for (size_t i = 0; i < arraySize; i += step)
				{
					arr[i].id *= 2;
				}

				auto t1 = std::chrono::high_resolution_clock::now();
				auto time = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

				timeArray.push_back(time);
			}

			std::sort(timeArray.begin(), timeArray.end());

			int64_t sum = 0;
			for (size_t i = 1; i < timeArray.size() - 1; ++i)
			{
				sum += timeArray[i];
			}

			double avg = (double)sum / (timeArray.size() - 2);
			resultsA.push_back((float)avg);
		}

		hasA = true;
	}

	if (ImGui::Button("Trash cache - Transform pointer"))
	{
		resultsB.clear();
		resultsB.reserve(11);

		if (samples2 < 1) samples2 = 1;
		const size_t N = static_cast<size_t>(samples2);

		struct Transform { float matrix[16]; };
		struct GameObject3DLike
		{
			std::unique_ptr<Transform> local;
			int id;
		};
		const size_t arraySize = 100000;
		auto transforms = std::make_unique<Transform[]>(arraySize);
		auto arr = std::make_unique<GameObject3DLike[]>(arraySize);

		for (size_t i = 0; i < arraySize; ++i)
		{
			arr[i].id = static_cast<int>(i);
			arr[i].local = std::make_unique<Transform>();
		}

		for (size_t step = 1; step <= 1024; step *= 2)
		{
			std::vector<int64_t> timeArray;
			timeArray.reserve(N);

			for (size_t j = 0; j < N; ++j)
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
			{
				sum += timeArray[k];
			}

			double avg = (double)sum / (double)(timeArray.size() - 2);
			resultsB.push_back((float)avg);
		}

		hasB = true;
	}

	//red
	if (hasA && !resultsA.empty())
	{
		ImGui::Text("Transform local");
		ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(1, 0, 0, 1)); 

		ImGui::PlotLines(
			"##PlotA",
			resultsA.data(),
			(int)resultsA.size(),
			0,
			nullptr,
			0.0f,
			*std::max_element(resultsA.begin(), resultsA.end()),
			ImVec2(0, 200)
		);

		ImGui::PopStyleColor();
	}

	//green
	if (hasB && !resultsB.empty())
	{
		ImGui::Text("Transform pointer");
		ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0, 1, 0, 1)); 

		ImGui::PlotLines(
			"##PlotB",
			resultsB.data(),
			(int)resultsB.size(),
			0,
			nullptr,
			0.0f,
			*std::max_element(resultsB.begin(), resultsB.end()),
			ImVec2(0, 200)
		);

		ImGui::PopStyleColor();
	}

	ImGui::End();


	ImGui::Render();

	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer);

	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_FRect dst{};
	dst.x = x;
	dst.y = y;
	SDL_GetTextureSize(texture.GetSDLTexture(), &dst.w, &dst.h);
	SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_FRect dst{};
	dst.x = x;
	dst.y = y;
	dst.w = width;
	dst.h = height;
	SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
