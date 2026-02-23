#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>) //If we're in debug mode and leak detector is available(File vlh.h exist), include it
#include <vld.h>
#endif

#if _DEBUG
	#if __has_include(<vld.h>)
	#pragma message(">>> VLD HEADER FOUND - including vld.h")
	#include <vld.h>
	#else
	#pragma message(">>> VLD HEADER NOT FOUND")
	#endif
#endif


#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "Scene.h"
#include "RenderComponent.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load() //Load is static so other files can't call it, only main.cpp can
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene(); // returns a reference to the scene that is stored in the scene manager

	//Create a game object, set it background and add it to the scene
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("background.png"); //I add render component to the game object and set the texture to background.png, because I want to be able to render the background, and I want to use the render component to do that, because it's responsible for rendering the texture of the game object, and I want to keep the rendering logic separate from the game object logic, and also because I want to be able to change the texture of the game object without changing its position or other properties, and also because I want to be able to reuse the render component for other game objects that have different textures but similar rendering logic
	scene.Add(std::move(go));

	//Create a game object, set it logo and add it to the scene //because of std::move, the go variable is now empty, so we can reuse it to create another game object, it's unique pointer so we don't need to delete it
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("logo.png");
	go->SetWorldPosition(358, 180);
	scene.Add(std::move(go));

	//Create a text object, set the text and add it to the scene
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto goText = std::make_unique<dae::GameObject>();
	goText->SetWorldPosition(292, 20);
	goText->AddComponent<dae::TextComponent>(
		font,
		"Programming 4 Assignment",
		SDL_Color{ 255, 255, 0, 255 }
	);
	scene.Add(std::move(goText));

	goText = std::make_unique<dae::GameObject>();
	goText->SetWorldPosition(292, 80);
	goText->AddComponent<dae::TextComponent>(
		font,
		"0 FPS",
		SDL_Color{ 255, 255, 0, 255 }
	);
	goText->AddComponent<dae::FPSComponent>();
	scene.Add(std::move(goText));

}

int main(int, char*[]) {
	/*int* leak = new int[100];
	leak[0] = 123;  */ 
	// leak on purpose to check if vld is working

#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
