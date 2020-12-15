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

#include "pf/plugin/game/entity/config.h"
#include "pf/basic/type/variable.h"

namespace pf_plugin {

namespace game {

namespace entity {

class PF_PLUGIN_API Interface {

 public:

   Interface() : mask_{0} {
     property_set_.clear();
   }
   virtual ~Interface() {}

 public:

   using variable_t = pf_basic::type::variable_t;
   using variable_array_t = pf_basic::type::variable_array_t;
   using closure_t = 
     std::function<void(entity::Interface *, const variable_array_t &)>;
   using callback_set = std::map<std::string, closure_t>;

 /* Interface defines. */
 public:

   //Move to a 2d position(not must be implement).
   virtual bool move_to(const v2 &) { return false; }

   //Move to a 3d position(not must be implement).
   virtual bool move_to(const v3 &) { return false; }

   //Convey to a scene 2d position(not must be implement).
   virtual bool convey_to(scene::Interface *, const v2 &) { return false; }

   //Convey to a scene 3d position(not must be implement).
   virtual bool convey_to(scene::Interface *, const v3 &) { return false; }

   //Convey to a scene by default position(not must be implement).
   virtual bool convey_to(scene::Interface *) { return false; }

   //The distance from a object(not must be implement).
   virtual int32_t distance(entity::Interface *et) const { return -1; }

   //Clone a new object like this(not must be implement).
   virtual void clone(entity::Interface &et) {}

   //Get the only id(default use the property "id").
   virtual variable_t get_id() const {
     return property_set_["id"];
   }

   //Set the only id(default use the property "id").
   virtual void set_id(const variable_t &id) {
     property_set_["id"] = id;
   }

   //Get the object type.
   virtual int8_t get_type() const {
     return property_set_["type"].get<int8_t>();
   }

 public:

   //Initialize for object.
   void init();
   
   //Set one property value of object.
   void set_property(uint8_t id, const variable_t &value) {
     auto oval = property_set_[id];
     property_set_[id] = value;
     on_property_changed(oval, value);
   }

   //Get one property value of object.
   variable_t get_property(uint8_t id) {
     return property_set_[id];
   }

   //Set one mask of object.
   void set_mask(uint8_t id, bool flag) {
     if (id >= 64) return;
     mask_ = flag ? mask_ | (0x1LL << id) : mask_ & ~(0x1LL << id);
   }

   //Get one mask of object.
   bool get_mask(uint8_t id) const {
     if (id >= 64) return false;
     return (mask_ & (0x1LL << id)) != 0;
   }

 /* Events */
 public:
    
   //The property changed event.
   void on_property_changed(const variable_t &oval, const variable_t &nval) {
     variable_array_t params{oval, nval};
     for (auto it = property_changed_callback_set_.begin(); 
          it != property_changed_callback_set_.end(); ++it)
       (it->second)(this, params);
   }

   //Set a property changed event.
   void set_property_changed_event(const std::string &name, closure_t callback) {
     property_changed_callback_set_[name] = callback;
   }

   //Unset a property changed event.
   void unset_property_changed_event(const std::string &name) {
     property_changed_callback_set_.erase(name);
   }

 private:

   //The property set.
   std::map<uint8_t, variable_t> property_set_;

   //The property changed callback set.
   callback_set property_changed_callback_set_;

   //The mask(0-63).
   int64_t mask_;

};

}; //namespace entity

} //namespace game

} //namespace pf_plugin

#endif //PF_PLUGIN_GAME_ENTITY_INTERFACE_H_
