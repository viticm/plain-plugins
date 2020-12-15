/**
 * PLAIN FRAMEWORK ( https://github.com/viticm/plain )
 * $Id manager.h
 * @link https://github.com/viticm/plain for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2019/11/18 15:05
 * @uses The scene manager class.
*/
#ifndef PF_PLUGIN_GAME_SCENE_MANAGER_H_
#define PF_PLUGIN_GAME_SCENE_MANAGER_H_

#include "pf/plugin/game/scene/config.h"
#include "pf/plugin/game/scene/basic.h"

namespace pf_plugin {

namespace game {

namespace scene {

class PF_PLUGIN_API Manager {

 public:

   Manager() {}
   virtual ~Manager() {}

 public:

   Interface *create();

};

} //namespace scene

} //namespace game

} //namespace pf_plugin

#endif //PF_PLUGIN_GAME_SCENE_MANAGER_H_
