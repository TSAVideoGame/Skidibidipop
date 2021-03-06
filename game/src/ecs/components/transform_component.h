#ifndef SKIDIBIDIBOP_GAME_ECS_COMPONENTS_TRANSFORM_COMPONENT
#define SKIDIBIDIBOP_GAME_ECS_COMPONENTS_TRANSFORM_COMPONENT

#include "entity.h"
#include <cstdint>
#include <unordered_map>
#include "component.h"

namespace Game
{
  namespace ECS
  {
    namespace Components
    {
      /*
       * ========================================
       * Transform Component
       *
       * Has data related to position
       * ========================================
       */
      class TransformManager : public Component
      {
      public:
        TransformManager();
        ~TransformManager();
        // All the data is stored in one buffer
        struct Data
        {
          std::uint32_t instances; // Current instances
          std::uint32_t allocated; // Allocated instances
          void* buffer;

          Entity* entity;
          std::uint32_t* x;
          std::uint32_t* y;
        };

        // An instance of the component
        struct Instance
        {
          std::uint32_t index;
        };

        Instance get_instance(const Entity&);

        std::uint32_t get_x(Instance&);
        void          set_x(Instance&, std::uint32_t);

        std::uint32_t get_y(Instance&);
        void          set_y(Instance&, std::uint32_t);

        Instance add_component(const Entity&);
        void destroy_component(Instance&);
        // TODO: Destroy component when entity is destroyed
      private:
        Data data;
        // Given an entity id, this is how we will get the instance index
        std::unordered_map<Entity, std::uint32_t> map;

        void allocate(std::uint32_t);
      };
      
      // Register the transform component
      extern Manager::RegisterComponent<TransformManager> transform_manager;
    };
  };
};

#endif
