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
	static bool hasResult2 = false;
	ImGui::Begin("Exercise 1");
	ImGui::Text("Pick number of samples");\
	static int samples = 1000000;
	ImGui::InputInt("Samples", &samples);
	if (ImGui::Button("Trash the cache"))
	{
		results.clear();
		results.reserve(samples);
		auto arr = std::make_unique<int[]>(samples);
		for (int i = 0; i < samples; ++i)
		{
			arr[i] = i;
		}

		for (size_t step = 1; step <= 1024; step *= 2)
		{
			auto t0 = std::chrono::high_resolution_clock::now();

			for (size_t i = 0; i < samples; i += step)
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
	ImGui::Text("Pick number of samples");\
	static int samples2 = 1000000;
	ImGui::InputInt("Samples", &samples2);

	

	if (ImGui::Button("Trash the cache with GameObject3D"))
	{
		results2.clear();
		int N = samples2;

		struct Transform { float matrix[16]; };

		struct GameObject3DLike
		{
			Transform* local;
			int id;
		};

		
		auto arr = std::make_unique<GameObject3DLike[]>(N);
		for (int i = 0; i < N; ++i)
		{
			arr[i].id = i;
		}

		for (size_t step = 1; step <= 1024; step *= 2)
		{
			auto t0 = std::chrono::high_resolution_clock::now();

			for (size_t i = 0; i < N; i += step)
				arr[i].id *= 2;

			auto t1 = std::chrono::high_resolution_clock::now();

			auto time = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

			results2.push_back(static_cast<float>(time));

		}
		hasResult2 = true;
	}

	if (hasResult2 && !results2.empty())
	{
		ImGui::PlotLines(
			"Execution Time (us)",
			results2.data(),
			static_cast<int>(results2.size()),
			0,
			nullptr,
			0.0f,
			*std::max_element(results2.begin(), results2.end()),
			ImVec2(0, 200)
		);
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
