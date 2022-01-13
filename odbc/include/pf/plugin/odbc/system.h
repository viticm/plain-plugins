/**
 * PLAIN FRAMEWORK ( https://github.com/viticm/plainframework )
 * $Id system.h
 * @link https://github.com/viticm/plainframework for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.it@gmail.com>
 * @date 2017/07/04 16:23
 * @uses the db system class, to provide some convenient operation functions.
 */
#ifndef PF_PLUGIN_ODBC_SYSTEM_H_
#define PF_PLUGIN_ODBC_SYSTEM_H_

#include "pf/plugin/odbc/config.h"
#include "pf/db/interface.h"
#include "pf/basic/tinytimer.h"

namespace pf_plugin {

namespace odbc {

class PF_PLUGIN_API System : public pf_db::Interface {
 public:
   enum dboption_type_t {
     kDBOptionTypeLoad,
     kDBOptionTypeSave, //insert and update
     kDBOptionTypeAddNew,
     kDBOptionTypeDelete,
     kDBOptionTypeQuery,
     kDBOptionTypeInitEmpty, //db at init state
   };
#if OS_WIN
   enum odbc_error_t {
     kODBCErrorSamePrimaryKey = 2601, //repeat primary key
   };
#elif OS_UNIX
   enum odbc_error_t {
     kODBCErrorSamePrimaryKey = 1026, //repeat primary key
   };
#endif

 public:
   System();
   virtual ~System();

 public:
   virtual bool init();
   virtual bool query(const std::string &sql_str);
   virtual bool fetch(int32_t orientation = 1, int32_t offset = 0);
   virtual int32_t get_affectcount() const;
   virtual bool check_db_connect(bool directly);
   bool isready() const { return isready_; };
   virtual bool getresult() const;
   virtual int32_t get_columncount() const;
   virtual const char *get_columnname(int32_t column_index) const;

 public:
   virtual float get_float(int32_t column_index, int32_t &error_code);
   virtual int64_t get_int64(int32_t column_index, int32_t &error_code);
   virtual uint64_t get_uint64(int32_t column_index, int32_t &error_code);
   virtual int32_t get_int32(int32_t column_index, int32_t &error_code);
   virtual uint32_t get_uint32(int32_t column_index, int32_t &error_code);
   virtual int16_t get_int16(int32_t column_index, int32_t &error_code);
   virtual uint16_t get_uint16(int32_t column_index, int32_t &error_code);
   virtual int8_t get_int8(int32_t column_index, int32_t &error_code);
   virtual uint8_t get_uint8(int32_t column_index, int32_t &error_code);
   virtual int32_t get_string(int32_t column_index, 
                              char *buffer, 
                              int32_t buffer_length, 
                              int32_t &error_code);
   virtual int32_t get_field(int32_t column_index, 
                             char *buffer, 
                             int32_t buffer_length, 
                             int32_t &error_code);
   virtual int32_t get_binary(int32_t column_index, 
                              char *buffer, 
                              int32_t buffer_length, 
                              int32_t &error_code);
   virtual int32_t get_binary_withdecompress(int32_t column_index, 
                                             char *buffer, 
                                             int32_t buffer_length, 
                                             int32_t &error_code);
   virtual const char *get_data(
       int32_t column_index, const char *_default) const;
   virtual db_columntype_t gettype(int32_t column_index);

 public:
   uint32_t get_result_count();
   int get_error_code();
   const char *get_error_message();
   virtual bool add_new(const std::string &sql_str);
   virtual bool _delete(const std::string &sql_str);
   virtual bool save(const std::string &sql_str);

 protected:
   int32_t result_count_;
   bool result_;
   dboption_type_t op_type_;
   std::unique_ptr<odbc::Interface> odbc_interface_;
   pf_basic::TinyTimer timer_;

 protected:
   int32_t get_internal_affect_count();
   bool is_prepare();
   bool long_load();
   bool long_save();
};

}; //namespace odbc

}; //namespace pf_plugin

#endif //PF_PLUGIN_ODBC_SYSTEM_H_
