/**
 * PLAIN FRAMEWORK ( https://github.com/viticm/plain )
 * $Id basic.h
 * @link https://github.com/viticm/plain for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2020/03/17 14:29
 * @uses The game scene basic class.
*/
#ifndef PF_PLUGIN_GAME_SCENE_BASIC_H_
#define PF_PLUGIN_GAME_SCENE_BASIC_H_

#include "pf/plugin/game/scene/config.h"
#include "pf/plugin/game/scene/Interface.h"

namespace pf_plugin {

namespace game {

namespace scene {

class PF_PLUGIN_API Basic : public Interface {

 public:

   Basic() {}
   virtual ~Basic() {}

};

} //namespace scene

} //namespace game

} //namespace pf_plugin


#endif //PF_PLUGIN_GAME_SCENE_BASIC_H_
