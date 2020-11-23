#ifndef SKIDIBIDIBOP_EDITOR_UTILITY
#define SKIDIBIDIBOP_EDITOR_UTILITY

#include <string>
#include "sdlw.h"

namespace Editor
{
  namespace Utility
  {
    /*
     * ========================================
     * void create_text_center
     *
     * Creates centered text for a texture and destRect
     * ========================================
     */
    SDLW::Texture* create_text_center(SDLW::Renderer*, const std::string& text, SDL_Rect& destRect, int w, int h, SDL_Color);
  };
};

#endif
