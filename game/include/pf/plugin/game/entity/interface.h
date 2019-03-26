/**
 * PLAIN FRAMEWORK ( https://github.com/viticm/plain )
 * $Id interface.h
 * @link https://github.com/viticm/plain for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2019/03/25 11:26
 * @uses The game world entity interface class.
*/
#ifndef PF_PLUGIN_GAME_ENTITY_INTERFACE_H_
#define PF_PLUGIN_GAME_ENTITY_INTERFACE_H_

namespace pf_plugin {

namespace game {

namespace entity {

class PF_PLUGIN_API Interface {

 public:

   Interface();
   virtual ~Interface();

 public:

   void init();


}

} //namespace entity

} //namespace game

} //namespace pf_plugin

#endif //PF_PLUGIN_GAME_ENTITY_INTERFACE_H_
