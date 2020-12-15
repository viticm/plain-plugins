/**
 * PLAIN FRAMEWORK ( https://github.com/viticm/plain )
 * $Id interface.h
 * @link https://github.com/viticm/plain for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2019/04/25 13:40
 * @uses The plugin game scene interface class.
*/
#ifndef PF_PLUGIN_GAME_SCENE_INTERFACE_H_
#define PF_PLUGIN_GAME_SCENE_INTERFACE_H_

#include "pf/plugin/game/scene/config.h"
#include "pf/net/packet/interface.h"
#include "pf/support/helpers.h"

namespace pf_plugin {

namespace game {

namespace scene {

class PF_PLUGIN_API Interface {

 public:

   Interface() {}
   virtual ~Interface() {}

 public:

   using et_hash_t = std::map<variable_t, std::unique_ptr< entity::Interface >>;

 /* Interface defines */
 public:

   //Clone new scene like this(not must be implement).
   virtual void clone(scene::Interface &) {}

   //Get the scene id.
   virtual int32_t get_id() const = 0;

   //Get the scene line.
   virtual uint8_t get_line() const = 0;

   //Get the scene max line.
   virtual uint8_t get_max_line() const = 0;

   //Get the scene if can enter it(not must be implement).
   virtual bool can_enter(entity::Interface *) { return true; }

   //An entity exit from this scene.
   virtual void exit(entity::Interface *) = 0;

   //move to a 2d position(not must be implement).
   virtual bool move_to(entity::Interface *, const v2 &) { return false; }

   //move to a 3d position(not must be implement).
   virtual bool move_to(entity::Interface *, const v3 &) { return false; }

   //Broadcast to all player in this scene(not must be implement).
   virtual void broadcast(pf_net::packet::Interface *) {}

   //Create a entity object in this scene.
   virtual entity::Interface *create_entity(
       std::function<entity::Interface * ()> func) {
     using namespace pf_support;
     auto et = func();
     if (is_null(et)) return;
     auto id = et->get_id();
     if (empty(id)) return;
     auto type = et->type();
     if (!is_null(et_list_[type][id])) return;
     et_list_[type][id] = std::move(et);
     return et_list_[type][id];
   }

 protected:

   //The all entity list.
   std::map<int8_t, et_hash_t> et_list_;


};

} //namespace scene

} //namespace game

} //namespace pf_plugin

#endif //PF_PLUGIN_GAME_SCENE_INTERFACE_H_
