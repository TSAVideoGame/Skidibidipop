#include "window.h"
#include "constants.h"
#include <SDL2/SDL_ttf.h>

/*
 * ========================================
 * Data Members
 * ========================================
 */
bool FFM::ChunkEditor::Window::running = true;
SDLW::Window* FFM::ChunkEditor::Window::window = nullptr;
SDLW::Renderer* FFM::ChunkEditor::Window::renderer = nullptr;
SDLW::Texture* FFM::ChunkEditor::Window::spritesheet = nullptr;
SDLW::Texture* FFM::ChunkEditor::Window::background = nullptr;
FFM::ChunkEditor::Inputs FFM::ChunkEditor::Window::inputs = {false, false, 0, 0, 0, 0, 0, 0, 0, false, false, false, false};

FFM::ChunkEditor::Tools::Manager* FFM::ChunkEditor::Window::tool_manager = nullptr;

FFM::Data::Types::Chunk FFM::ChunkEditor::Window::data;
FFM::ChunkEditor::Tools::Base* FFM::ChunkEditor::Window::selected_tool = nullptr;
/*
 * ========================================
 * Init / Close
 * ========================================
 */
void FFM::ChunkEditor::Window::init()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();

  window = new SDLW::Window("FFM Chunk Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Constants::Window.TOOL_WIDTH + Constants::Window.VIEW_WIDTH, Constants::Window.VIEW_HEIGHT, 0);
  renderer = new SDLW::Renderer(window);
  spritesheet = new SDLW::Texture("res/spritesheet.png", renderer);

  tool_manager = new Tools::Manager(renderer);

  update_background();
}

void FFM::ChunkEditor::Window::close()
{
  delete background;

  delete tool_manager;

  delete spritesheet;
  delete renderer;
  delete window;

  TTF_Quit();
  SDL_Quit();
}

/*
 * ========================================
 * Input / Update / Draw
 * ========================================
 */
void FFM::ChunkEditor::Window::input()
{
  // Update old mouse stuff
  inputs.old_mouse_down= inputs.mouse_down;
  inputs.old_mouse_x = inputs.mouse_x;
  inputs.old_mouse_y = inputs.mouse_y;
  inputs.mouse_wheel_y = 0; // Also set mouse wheel to 0

  // Reset arrows (they are only true on release)
  inputs.up = false;
  inputs.right = false;
  inputs.down = false;
  inputs.left = false;

  // Handle events
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
      /*
      case SDL_KEYDOWN:
      {
        switch (e.key.keysym.sym)
        {
          case SDLK_UP:
          {
            inputs.up = true;

            break;
          }
          case SDLK_RIGHT:
          {
            inputs.right = true;

            break;
          }
          case SDLK_DOWN:
          {
            inputs.down = true;

            break;
          }
          case SDLK_LEFT:
          {
            inputs.left = true;

            break;
          }
        }
        break;
      }
      */
      case SDL_KEYUP:
      {
        switch (e.key.keysym.sym)
        {
          case SDLK_UP:
          {
            inputs.up = true;

            break;
          }
          case SDLK_RIGHT:
          {
            inputs.right = true;

            break;
          }
          case SDLK_DOWN:
          {
            inputs.down = true;

            break;
          }
          case SDLK_LEFT:
          {
            inputs.left = true;

            break;
          }
        }
        break;
      }
      case SDL_MOUSEBUTTONDOWN:
      {
        switch (e.button.button)
        {
          case SDL_BUTTON_LEFT:
          {
            inputs.mouse_down = true;
            if (!inputs.old_mouse_down)
              SDL_GetMouseState(&inputs.click_mouse_x, &inputs.click_mouse_y);

            break;
          }
        }

        break;
      };
      case SDL_MOUSEBUTTONUP:
      {
        switch (e.button.button)
        {
          case SDL_BUTTON_LEFT:
          {
            inputs.mouse_down = false;

            break;
          }
        }

        break;
      };
      case SDL_MOUSEWHEEL:
      {
        inputs.mouse_wheel_y = e.wheel.y;

        break;
      };
      case SDL_WINDOWEVENT:
      {
        if (SDL_GetWindowID(window->get_SDL()) != e.window.windowID)
        {
          inputs.mouse_down = false;
        }

        break;
      };
    }
  }

  SDL_GetMouseState(&inputs.mouse_x, &inputs.mouse_y);
}

void FFM::ChunkEditor::Window::update()
{
  // Mouse states
  tool_manager->update(MouseState::HOVER);

  if (inputs.mouse_down)
  {
    if (!inputs.old_mouse_down) // Click
    {
      tool_manager->update(MouseState::CLICK);
    }

    tool_manager->update(MouseState::DRAG);
  }

  // Release mouse
  if (!inputs.mouse_down && inputs.old_mouse_down)
  {
    tool_manager->update(MouseState::RELEASE); 
  }

  // Scroll
}

// Helper draw functions
static void draw_objects(SDLW::Renderer* renderer)
{
  for (std::vector<FFM::Data::Types::Chunk::Object>::iterator i = FFM::ChunkEditor::Window::data.objects.begin(); i < FFM::ChunkEditor::Window::data.objects.end(); ++i)
  {
    FFM::Data::Types::Chunk::Vertex v = FFM::ChunkEditor::Window::data.vertices[i->vertex];
    SDL_Rect src_rect = {i->id * 32, 0, 32, 32};
    SDL_Rect dest_rect = {v.x + FFM::ChunkEditor::Constants::Window.TOOL_WIDTH, v.y, 32, 32};
    renderer->copy(FFM::ChunkEditor::Window::get_spritesheet(), &src_rect, &dest_rect);
  }
}

static void draw_collisions(SDLW::Renderer* renderer)
{
  // Draw Vertices
  // This is tricky because we don't want to draw the object/monster/npc vertices
  // Also this is inefficient
  for (std::size_t i = 0; i < FFM::ChunkEditor::Window::data.vertices.size(); ++i)
  {
    // Make sure vertex isn't being used by non-line thing
    bool found = false;
    for (std::size_t oi = 0; oi < FFM::ChunkEditor::Window::data.objects.size(); ++oi)
    {
      if (FFM::ChunkEditor::Window::data.objects[oi].vertex == i)
      {
        found = true;
        break;
      }
    }
    if (found)
      continue;
    for (std::size_t mi = 0; mi < FFM::ChunkEditor::Window::data.enemies.size(); ++mi)
    {
      if (FFM::ChunkEditor::Window::data.enemies[mi].vertex == i)
      {
        found = true;
        break;
      }
    }
    if (found)
      continue;
    for (std::size_t ni = 0; ni < FFM::ChunkEditor::Window::data.npcs.size(); ++ni)
    {
      if (FFM::ChunkEditor::Window::data.npcs[ni].vertex == i)
      {
        found = true;
        break;
      }
    }
    if (found)
      continue;


    FFM::Data::Types::Chunk::Vertex v = FFM::ChunkEditor::Window::data.vertices[i];
    renderer->set_draw_color(0, 0, 0, 255);
    SDL_Rect dest_rect = {(v.x - 5) + FFM::ChunkEditor::Constants::Window.TOOL_WIDTH, v.y - 5, 9, 9};
    SDL_RenderFillRect(renderer->get_SDL(), &dest_rect);
  }
}

// Actual draw function
void FFM::ChunkEditor::Window::draw()
{
  renderer->set_draw_color(255, 255, 255, 255);
  renderer->clear();

  // Draw view area
  if (background)
  {
    SDL_Rect d_rect = {Constants::Window.TOOL_WIDTH, 0, Constants::Window.VIEW_WIDTH, Constants::Window.VIEW_HEIGHT};
    renderer->copy(background, nullptr, &d_rect);
  }
  draw_collisions(renderer);
  draw_objects(renderer);

  // Draw tool stuff
  SDL_Color c = tool_manager->get_color();
  renderer->set_draw_color(c.r, c.g, c.b, 255);
  SDL_Rect toolbar = {0, 0, Constants::Window.TOOL_WIDTH, Constants::Window.VIEW_HEIGHT};
  SDL_RenderFillRect(renderer->get_SDL(), &toolbar);
  tool_manager->draw();

  renderer->present();
}

/*
 * ========================================
 * Other Methods
 * ========================================
 */
void FFM::ChunkEditor::Window::update_background()
{
  if (background != nullptr)
    delete background;

  std::string file_name = "res/chunk_images/" +std::to_string(data.background_id) + ".png";
  background = new SDLW::Texture(file_name.c_str(), renderer);
}

/*
 * ========================================
 * Get / Set
 * ========================================
 */
bool FFM::ChunkEditor::Window::is_running() { return running; }
FFM::ChunkEditor::Inputs FFM::ChunkEditor::Window::get_inputs() { return inputs; }
SDLW::Texture* FFM::ChunkEditor::Window::get_spritesheet() { return spritesheet; }

/*
 * ========================================
 * Unused methods
 * ========================================
 */
FFM::ChunkEditor::Window::Window() {}
FFM::ChunkEditor::Window::~Window() {}
