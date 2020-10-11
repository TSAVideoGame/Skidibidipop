#ifndef SKIDIBIDIBOP_GAME_MAP
#define SKIDIBIDIBOP_GAME_MAP

#include "data.h"
#include "tile.h"
#include "sdl.h"
#include "core_object_manager.h"

// Full temporary until I get the Core::Object::Manager situated
namespace Game
{
  namespace Map
  {
    class Manager : public Core::Object::Manager
    {
    public:
      Manager(SDL::Renderer*, const Data::Types::Map&);
      void loadMap(const Data::Types::Map&);
    private:
      SDL::Renderer* renderer;
    };
  };
};

#endif
