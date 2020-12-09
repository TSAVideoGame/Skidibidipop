#include "core.h"
#include "constants.h"
#include "plugins.h"
#include "system.h"

/*
 * ========================================
 * Game::Core
 *
 * Handles game loop input, update, and
 * drawing
 * ========================================
 */

/*
 * ========================================
 * Data members
 * ========================================
 */
// Basic
SDLW::Window* Game::Core::window = nullptr;
SDLW::Renderer* Game::Core::renderer = nullptr;
SDLW::Texture* Game::Core::spritesheet = nullptr;
bool Game::Core::running = false;

/*
 * ========================================
 * Core::init
 * ========================================
 */
void Game::Core::init()
{
  SDL_Init(0);
  SDL_InitSubSystem(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  
  window = new SDLW::Window("Skidibidbop", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Constants.Window.width, Constants.Window.height, 0);
  renderer = new SDLW::Renderer(window);
  spritesheet = new SDLW::Texture("res/spritesheet.png", renderer);

  running = true;

  ECS::Systems::Manager::get_instance().init();

  // Testing plugins
  Plugins::Manager::get_instance().get_plugin<Plugins::Audio>()->add_music("res/music/TSA_test_1.wav");
  Plugins::Manager::get_instance().get_plugin<Plugins::Audio>()->play_music(0);
}

/*
 * ========================================
 * Core::close
 * ========================================
 */
void Game::Core::close()
{
  ECS::Systems::Manager::get_instance().close();

  delete spritesheet;
  delete renderer;
  delete window;

  SDL_QuitSubSystem(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  SDL_Quit();
}

void Game::Core::input()
{
  SDL_Event e;

  while (SDL_PollEvent(&e))
  {
    switch (e.type)
    {
      case SDL_QUIT:
      {
        running = false;

        break;
      }
    }
  }
}

void Game::Core::update()
{
  ECS::Systems::Manager::get_instance().update();
}

void Game::Core::draw()
{
  ECS::Systems::Manager::get_instance().draw();
}

// Getters / Setters
bool Game::Core::is_running() { return running; }

// Unused functions
Game::Core::Core() {}
Game::Core::~Core() {}
