/**
 * PLAIN FRAMEWORK ( https://github.com/viticm/plainframework )
 * $Id config.h
 * @link https://github.com/viticm/plainframework for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.it@gmail.com>
 * @date 2017/07/04 16:31
 * @uses the common db odbc base config
 */
#ifndef PF_PLUGIN_ODBC_CONFIG_H_
#define PF_PLUGIN_ODBC_CONFIG_H_

#include "pf/db/config.h"

#define HOST_LENGTH 30
#define CONNECTION_NAME_LENGTH 32
#define COLUMN_NAME_MAX 30 //column name max length
#define COLUMN_INFO_BUFFER_MAX (1024*8) //normal one column value length
#define COLUMN_VALUE_BUFFER_MAX (1024*400)
#define ERROR_MESSAGE_LENGTH_MAX 255
#define DB_ODBC_COLUMN_NAME_LENGTH_MAX 256

namespace pf_plugin {

namespace odbc {

class Interface;
class System;

};

}; //namespace pf_plugin

#endif //PF_PLUGIN_ODBC_CONFIG_H_
