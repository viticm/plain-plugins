#include "pf/basic/util.h"
#include "pf/basic/logger.h"
#include "pf/basic/time_manager.h"
#include "pf/plugin/odbc/interface.h"
#include "pf/plugin/odbc/system.h"

namespace pf_plugin {

namespace odbc {

System::System()
  : result_count_{0},
  result_{0},
  op_type_{kDBOptionTypeInitEmpty},
  odbc_interface_{nullptr} {
}

System::~System() {
}

bool System::init() {
  bool connected = true;
  auto odbc_interface = new odbc::Interface();
  Assert(odbc_interface);
  unique_move(pf_plugin::odbc::Interface, odbc_interface, odbc_interface_);
  connected = odbc_interface_->connect(
      name_.c_str(), username_.c_str(), password_.c_str());
  if (!connected) {
    SLOW_ERRORLOG(DB_MODULENAME,
                  "[db.odbc] (System::init) failed."
                  " connectionname: %s, username: %s, password: %s,"
                  " errormessage: %s",
                  name_.c_str(),
                  username_.c_str(),
                  password_.c_str(),
                  get_error_message());
  }
  isready_ = connected;
  return connected;
}


int32_t System::get_affectcount() const {
  return result_count_;
}

uint32_t System::get_result_count() {
  return result_count_;
}

bool System::fetch(int32_t orientation, int32_t offset) {
  if (is_null(odbc_interface_)) return false;
  bool result = odbc_interface_->fetch(orientation, offset);
  return result;
}

int32_t System::get_internal_affect_count() {
  if (is_null(odbc_interface_)) return 0;
  return odbc_interface_->get_affect_row_count();
}

bool System::is_prepare() {
  if (is_null(odbc_interface_)) return false;
  return odbc_interface_->is_prepare();
}

bool System::check_db_connect(bool directly) {
  Assert(odbc_interface_);
  bool result = true; //Error check with not connected.
  if (!odbc_interface_->is_connected()) {
    auto tickcount = TIME_MANAGER_POINTER->get_tickcount();
    if ((timer_.counting(tickcount) || directly) && odbc_interface_->connect()) {
      SLOW_DEBUGLOG("odbc",
        "[db.odbc] the connection reconnect successful!"
        " connection name: %s.",
        odbc_interface_->get_connection_name());
      timer_.cleanup();
      result = true;
    } else {
      if (!timer_.isstart()) {
        SLOW_WARNINGLOG(DB_MODULENAME,
          "[db.odbc] the connection lost, try connect after 5 seconds!"
          " connection name: %s.",
          odbc_interface_->get_connection_name());
        timer_.start(5000, tickcount);
      }
    }
  }
  return result;
}

bool System::getresult() const {
  return result_;
}

bool System::query(const std::string &sql_str) {
  if (!is_prepare()) return false;
  if (!odbc_interface_) return false;
  odbc_interface_->clear();
  op_type_ = kDBOptionTypeQuery;
  result_ = odbc_interface_->execute(sql_str);
  result_count_ = odbc_interface_->get_affect_row_count();
  return result_;
}

bool System::add_new(const std::string &sql_str) {
  if (!is_prepare()) return false;
  if (!odbc_interface_) return false;
  op_type_ = kDBOptionTypeAddNew;
  odbc_interface_->clear();
  result_ = odbc_interface_->execute(sql_str);
  result_count_ = odbc_interface_->get_affect_row_count();
  return result_;
}

bool System::_delete(const std::string &sql_str) {
  if (!is_prepare()) return false;
  if (!odbc_interface_) return false;
  op_type_ = kDBOptionTypeDelete;
  odbc_interface_->clear();
  result_ = odbc_interface_->execute(sql_str);
  result_count_ = odbc_interface_->get_affect_row_count();
  return result_;
}

bool System::save(const std::string &sql_str) {
  if (!is_prepare()) return false;
  if (!odbc_interface_) return false;
  op_type_ = kDBOptionTypeSave;
  odbc_interface_->clear();
  result_ = odbc_interface_->execute(sql_str);
  result_count_ = odbc_interface_->get_affect_row_count();
  return result_;
}

int32_t System::get_error_code() {
  return odbc_interface_->get_error_code();
}

const char *System::get_error_message() {
  return odbc_interface_->get_error_message();
}

int32_t System::get_columncount() const {
  int32_t result = 0;
  result = odbc_interface_->get_columncount();
  return result;
}

db_columntype_t System::gettype(int32_t column_index) {
  auto type = kDBColumnTypeString;
  auto typecode = odbc_interface_->get_type(column_index); 
  switch (typecode) {
    case SQL_CHAR:
      break;
    case SQL_VARCHAR:
      break;
    case SQL_LONGVARCHAR:
      break;
    case SQL_WCHAR:
      break;
    case SQL_WVARCHAR:
      break;
    case SQL_WLONGVARCHAR:
      break;
    case SQL_DECIMAL:
      break;
    case SQL_NUMERIC:
      type = kDBColumnTypeNumber;
      break;
    case SQL_SMALLINT:
      type = kDBColumnTypeInteger;
      break;
    case SQL_INTEGER:
      type = kDBColumnTypeInteger;
      break;
    case SQL_REAL:
      type = kDBColumnTypeNumber;
      break;
    case SQL_FLOAT:
      type = kDBColumnTypeNumber;
      break;
    case SQL_DOUBLE:
      type = kDBColumnTypeNumber;
      break;
    case SQL_BIT:
      type = kDBColumnTypeInteger;
      break;
    case SQL_TINYINT:
      type = kDBColumnTypeInteger;
      break;
    case SQL_BIGINT:
      type = kDBColumnTypeInteger;
      break;
    case SQL_TYPE_DATE:
      break;
    case SQL_TYPE_TIME:
      break;
    case SQL_TYPE_TIMESTAMP:
      break;
    case SQL_INTERVAL_MONTH:
      break;
    case SQL_INTERVAL_YEAR:
      break;
    case SQL_INTERVAL_YEAR_TO_MONTH:
      break;
    case SQL_INTERVAL_DAY:
      break;
    case SQL_INTERVAL_HOUR:
      break;
    case SQL_INTERVAL_MINUTE:
      break;
    case SQL_INTERVAL_SECOND:
      break;
    case SQL_INTERVAL_DAY_TO_HOUR:
      break;
    case SQL_INTERVAL_DAY_TO_MINUTE:
      break;
    case SQL_INTERVAL_DAY_TO_SECOND:
      break;
    case SQL_INTERVAL_HOUR_TO_MINUTE:
      break;
    case SQL_INTERVAL_HOUR_TO_SECOND:
      break;
    case SQL_INTERVAL_MINUTE_TO_SECOND:
      break;
    case SQL_GUID:
      break;
    case SQL_BINARY:
      break;
    case SQL_VARBINARY:
      break;
    case SQL_LONGVARBINARY:
      break;
    default:
      break;
  }
  return type;
}

float System::get_float(int32_t column_index, int32_t &error_code) {
  if (is_null(odbc_interface_)) return .0f;
  return odbc_interface_->get_float(column_index, error_code);
}

int64_t System::get_int64(int32_t column_index, int32_t &error_code) {
  if (is_null(odbc_interface_)) return -1;
  return odbc_interface_->get_int64(column_index, error_code);
}

uint64_t System::get_uint64(int32_t column_index, int32_t &error_code) {
  if (is_null(odbc_interface_)) return 0;
  return odbc_interface_->get_uint64(column_index, error_code);
}

int32_t System::get_int32(int32_t column_index, int32_t &error_code) {
  if (is_null(odbc_interface_)) return -1;
  return odbc_interface_->get_int32(column_index, error_code);
}

uint32_t System::get_uint32(int32_t column_index, int32_t &error_code) {
  if (is_null(odbc_interface_)) return 0;
  return odbc_interface_->get_uint32(column_index, error_code);
}
   
int16_t System::get_int16(int32_t column_index, int32_t &error_code) {
  if (is_null(odbc_interface_)) return -1;
  return odbc_interface_->get_int16(column_index, error_code);
}

uint16_t System::get_uint16(int32_t column_index, int32_t &error_code) {
  if (is_null(odbc_interface_)) return 0;
  return odbc_interface_->get_uint16(column_index, error_code);
}
   
int8_t System::get_int8(int32_t column_index, int32_t &error_code) {
  if (is_null(odbc_interface_)) return -1;
  return odbc_interface_->get_int8(column_index, error_code);
}

uint8_t System::get_uint8(int32_t column_index, int32_t &error_code) {
  if (is_null(odbc_interface_)) return 0;
  return odbc_interface_->get_uint8(column_index, error_code);
}
 
int32_t System::get_string(int32_t column_index, 
                          char *buffer, 
                          int32_t buffer_length, 
                          int32_t &error_code) {
  if (is_null(odbc_interface_)) return -1;
  return odbc_interface_->
    get_string(column_index, buffer, buffer_length, error_code);
}

int32_t System::get_field(int32_t column_index, 
                         char *buffer, 
                         int32_t buffer_length, 
                         int32_t &error_code) {
  if (is_null(odbc_interface_)) return -1;
  return odbc_interface_->
    get_field(column_index, buffer, buffer_length, error_code);
}

int32_t System::get_binary(int32_t column_index, 
                          char *buffer, 
                          int32_t buffer_length, 
                          int32_t &error_code) {
  if (is_null(odbc_interface_)) return -1;
  return odbc_interface_->
    get_binary(column_index, buffer, buffer_length, error_code);
}

int32_t System::get_binary_withdecompress(int32_t column_index, 
                                         char *buffer, 
                                         int32_t buffer_length, 
                                         int32_t &error_code) {
  if (is_null(odbc_interface_)) return -1;
  return odbc_interface_->
    get_binary_withdecompress(column_index, buffer, buffer_length, error_code);
}

const char *System::get_columnname(int32_t column_index) const {
  if (is_null(odbc_interface_)) return nullptr;
  return odbc_interface_->get_name(column_index);
}

const char *System::get_data(
  int32_t column_index, const char *_default) const {
  if (is_null(odbc_interface_)) return nullptr;
  return odbc_interface_->get_data(column_index, _default);
}

} //namespace odbc

} //namespace pf_plugin
