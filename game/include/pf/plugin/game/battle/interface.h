/**
 * PLAIN FRAMEWORK ( https://github.com/viticm/plain )
 * $Id interface.h
 * @link https://github.com/viticm/plain for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2019/08/19 20:18
 * @uses The game battle battle interface class.
*/
#ifndef PF_PLUGIN_GAME_BATTLE_INTERFACE_H_
#define PF_PLUGIN_GAME_BATTLE_INTREFACE_H_

#include "pf/plugin/game/battle/config.h"

namespace pf_plugin {

namespace game {

namespace battle {

class PF_PLUGIN_API Interface {

 public:

   Interface() {}
   virtual ~Interface() {} 

 public:

   virtual void init();

};

} //namespace battle

} //namespace game

} //namespace pf_plugin


#endif //PF_PLUGIN_GAME_BATTLE_INTREFACE_H_
