#include "pf/basic/logger.h"
#include "pf/basic/stringstream.h"
#include "pf/net/stream/input.h"
#include "pf/net/stream/output.h"
#include "pf/net/packet/dynamic.h"
#include "pf/net/packet/factorymanager.h"
#include "pf/basic/io.tcc"
#include "pf/sys/assert.h"
#include "pf/plugin/protocol/tlbb.h"

#define CANT_PEEK_TIMES_MAX 60


void binary_print(char c) {
  for (int8_t i = 0; i < 8; ++i) {
    if (c << i & 0x80)
      std::cout << '1';
    else
      std::cout << '0';
  }
  std::cout << std::endl;
}

using namespace pf_net;
using namespace pf_plugin::protocol;

std::string get_raw_string(char *str, uint16_t size) {
  std::cout << "size: " << size << std::endl;
  std::ostringstream out;
  out << '\"';
  out << std::hex;
  for (uint16_t i = 0; i < size; ++i) {
     // AND 0xFF will remove the leading "ff" in the output,
     //  // So that we could get "\xab" instead of "\xffab"
     out << "\\x" << (static_cast<short>(str[i]) & 0xff);
     //    
  }
  out << '\"';
  return out.str();    
}

bool is_little_endian() {
  union {
    int32_t a = 0x12345678;
    char b;
  } data;
  if (0x12 == data.b)
    return false;
  else if (0x78 == data.b)
    return true;
  return false;
}

union {
  char c[4];
  unsigned long l;
} endian_test = {{'l', '?', '?', 'b'}};
#define ENDIANNESS ((char)endian_test.l)

TLBBHead empty_header;
TLBBFoot empty_footer;

TLBB::TLBB() {
  //do nothing.
}

TLBB::~TLBB() {
  //do nothing.
}

bool TLBB::command(connection::Basic *connection, uint16_t count) {
  bool result = false;
  TLBBHead header;
  if (is_null(connection)) return false;
  stream::Input *istream = &connection->istream();
  packet::Interface *packet = nullptr;
  // Error times is check the invalid client connected.
  auto error_times = connection->get_error_times();
  if (error_times >= CANT_PEEK_TIMES_MAX) return false;
  if (connection->is_disconnect()) return false; //leave this to connection.
  bool clear_error{true};
  try {
    uint32_t i;
    for (i = 0; i < count; ++i) {
      memset((char *)&header, 0, sizeof(header));
      if (!istream || 0 == istream->size()) return true;
      if (!istream->peek((char *)&header, sizeof(header))) {
        //数据不能填充消息头
        connection->inc_error_times();
        clear_error = false;
        break;
      }
      tlbb_head_ntoh(header);
      // std::cout << "is_little_endian: " << is_little_endian() << std::endl;
      char tmp[512]{0};
      istream->peek(tmp, istream->size());
      auto tmp_b = get_raw_string(tmp, istream->size());
      std::cout << "l: " << ENDIANNESS << std::endl;
      std::cout << "binary: " << tmp_b << " size: " << istream->size() << std::endl;
      printf("header.id: %d\n", header.id);
      /**
      pf_basic::stringstream tmp2(tmp, istream->size());
      uint16_t mask{0};
      tmp2 >> mask;
      binary_print(((char *)(&mask))[0]);
      binary_print(((char *)(&mask))[1]);
      std::cout << std::hex << "mask: " << mask << " mask1: " << ntohs(0xAA55) << std::endl;
      **/
      if (empty_header.mask != header.mask) {
        Assert(false);
        return false;
      }
      if (!dynamic_ &&
          !NET_PACKET_FACTORYMANAGER_POINTER->
          is_valid_packet_id(header.id) &&
          !NET_PACKET_FACTORYMANAGER_POINTER->
          is_valid_dynamic_packet_id(header.id) &&
          !NET_PACKET_FACTORYMANAGER_POINTER->
          is_encrypt_packet_id(header.id)) {
        pf_basic::io_cerr("packet id error: %d", header.id);
        return false;
      }
      header.size = header.size - 3;
      if (!NET_PACKET_FACTORYMANAGER_POINTER->is_encrypt_packet_id(header.id) &&
          !connection->check_safe_encrypt())
        return false;
      try {
        //check packet length
        if (!NET_PACKET_FACTORYMANAGER_POINTER->
            is_valid_dynamic_packet_id(header.id) && !dynamic_) {
          if (header.size > 
            NET_PACKET_FACTORYMANAGER_POINTER->packet_max_size(header.id)) {
            char temp[FILENAME_MAX] = {0};
            snprintf(temp, 
                     sizeof(temp) - 1, 
                     "packet size error, packetid = %d", 
                     header.id);
            AssertEx(false, temp);
            return false;
          }
        }
        std::cout << "check size:" << istream->size() << "|" << (sizeof(TLBBHead) + header.size - 3 + sizeof(TLBBFoot)) << std::endl;
        //check packet size
        if (istream->size() < 
            sizeof(TLBBHead) + header.size - 3 + sizeof(TLBBFoot)) {
          //message not receive full
          break;
        }

        std::cout << "dynamic_: " << dynamic_ << std::endl;

        //create packet
        packet = 
          NET_PACKET_FACTORYMANAGER_POINTER->packet_create(header.id, dynamic_);
        if (nullptr == packet) return false;

        //packet info
        packet->set_index(static_cast<int8_t>(header.index));
        packet->set_id(header.id);
        packet->set_size(header.size);

        //read packet
        result = istream->skip(sizeof(TLBBHead));
        result = result ? packet->read(*istream) : result;
        if (false == result) {
          NET_PACKET_FACTORYMANAGER_POINTER->packet_remove(packet);
          return result;
        }
        //foot.

        TLBBFoot footer;
        if (!istream->peek((char *)&footer, sizeof(footer))) {
          return false;
        }
        tlbb_foot_ntoh(footer);
        istream->skip(sizeof(TLBBFoot));
        if (footer.mask != empty_footer.mask) {
          Assert(false);
          return false;
        }
        bool needremove = true;
        bool exception = false;
        uint32_t executestatus{kPacketExecuteStatusContinue};
        try {
          //connection->resetkick();
          try {
            if (connection->get_param("routing") != "") {
              if (!connection->forward(packet))
                executestatus = kPacketExecuteStatusError;
            } else {
              executestatus = packet->execute(connection);
            }
          } catch(...) {
            SaveErrorLog();
            executestatus = kPacketExecuteStatusError;
          }
          if (kPacketExecuteStatusError == executestatus) {
            if (packet) 
              NET_PACKET_FACTORYMANAGER_POINTER->packet_remove(packet);
            return false;
          } else if (kPacketExecuteStatusBreak == executestatus) {
            if (packet) 
              NET_PACKET_FACTORYMANAGER_POINTER->packet_remove(packet);
            break;
          } else if (kPacketExecuteStatusContinue == executestatus) {
            //continue read last packet
          } else if (kPacketExecuteStatusNotRemove == executestatus) {
            needremove = false;
          } else if (kPacketExecuteStatusNotRemoveError == executestatus) {
            return false;
          } else {
            //unknown status
          }
        } catch(...) {
          SaveErrorLog();
          exception = true;
        }
        if (packet && needremove) { 
          NET_PACKET_FACTORYMANAGER_POINTER->packet_remove(packet);
        }
        if (exception) return false;
      } catch(...) {
        SaveErrorLog();
        return false;
      }
    }
  } catch(...) {
    SaveErrorLog();
    return false;
  }
  if (clear_error) connection->set_error_times(0);
  return true;
}

bool TLBB::compress(connection::Basic *connection, 
                    char *uncompress_buffer, 
                    char *compress_buffer) {
  return false;
  /*
  stream::Input &istream = connection->istream();
  //stream::Output &ostream = connection->ostream();
  stream::Input &istream_compress = connection->istream_compress();
  //Write the compress input stream decode result to the normal.
  pf_util::compressor::Assistant *assistant = nullptr;
  assistant = istream.getcompressor()->getassistant();    
  if (!assistant->isenable() || is_null(&istream_compress)) return false;
  uint16_t compressheader = 0;
  char packetheader[NET_PACKET_HEADERSIZE] = {0};
  uint16_t packetid = 0;
  uint32_t packetcheck = 0;
  uint32_t packetsize = 0;
  uint32_t size = 0;
  uint32_t result = 0;
  if (is_null(uncompress_buffer) || is_null(compress_buffer)) {
    Assert(false);
    return false;
  }
  memset(uncompress_buffer, 0, NET_CONNECTION_UNCOMPRESS_BUFFER_SIZE);
  memset(compress_buffer, 0, NET_CONNECTION_COMPRESS_BUFFER_SIZE);
  do {
    if (!istream_compress.peek(
          reinterpret_cast<char *>(&compressheader), 
          sizeof(compressheader))) {
      break;
    }
    if (static_cast<int16_t>(compressheader) < 0) {
      compressheader &= 0x7FFF;
      uint32_t totalsize = sizeof(compressheader) + compressheader;
      if (size < totalsize) break;
      result = istream_compress.read(uncompress_buffer, totalsize);
      if (0 == result) return false;
      uint32_t outsize = 0;
      bool _result = istream.getcompressor()->decompress(
          uncompress_buffer, compressheader, compress_buffer, outsize);
      if (!_result) {
        SLOW_ERRORLOG(
            NET_MODULENAME,
            "[net.protocol] (TLBB::compress)"
            " istream->getcompressor()->decompress fail");
        return false;
      }
      if (istream.encrypt_isenable()) {
        istream.getencryptor()
          ->decrypt(compress_buffer, compress_buffer, outsize);
      }
      result = istream.write(compress_buffer, outsize);
      if (result != outsize) {
        SLOW_ERRORLOG(
            NET_MODULENAME,
            "[net.protocol] (TLBB::compress)"
            " istream.write fail result: %d, outsize: %d",
            result,
            outsize);
        return false;
      }
    } else {
      if (!istream_compress.peek(&packetheader[0], sizeof(packetheader)))
        break;
      memcpy(&packetid, &packetheader[0], sizeof(packetid));
      memcpy(&packetcheck, 
             &packetheader[sizeof(packetid)], 
             sizeof(packetcheck));
      if (!NET_PACKET_FACTORYMANAGER_POINTER->is_valid_packet_id(packetid) &&
          !NET_PACKET_FACTORYMANAGER_POINTER
          ->is_valid_dynamic_packet_id(packetid)) {
        SLOW_ERRORLOG(
            NET_MODULENAME,
            "[net.connection] (TLBB::process_compressinput)"
            " packetid not valid id: %d",
            packetid);
        return false;
      }
      packetsize = NET_PACKET_GETLENGTH(packetcheck);
      size = istream_compress.size();
      if (!NET_PACKET_FACTORYMANAGER_POINTER
          ->is_valid_dynamic_packet_id(packetid)) {
        uint32_t sizemax = 
          NET_PACKET_FACTORYMANAGER_POINTER->packet_max_size(packetid);
        if (packetsize > sizemax) {
          SLOW_ERRORLOG(
              NET_MODULENAME,
              "[net.connection] (TLBB::process_compressinput)"
              " packet size more than max size(%d, %d, %d)",
              packetid,
              packetsize,
              sizemax);
          return false;
        }
      }
      //Waiting for full.
      uint32_t totalsize = NET_PACKET_HEADERSIZE + packetsize;
      if (size < totalsize) break;

      //Read it.
      result = istream_compress.read(uncompress_buffer, totalsize);
      if (0 == result) return false;
      result = istream.write(uncompress_buffer, totalsize);
      if (result != totalsize) {
        SLOW_ERRORLOG(
            NET_MODULENAME,
            "[net.protocol] (TLBB::compress)"
            " read write data size not equal(%d, %d)",
            totalsize,
            result);
        return false;
      }
      if (NET_PACKET_FACTORYMANAGER_POINTER->is_encrypt_packet_id(packetid))
        break;
    }
  } while(true);
  return true;
  */
}

bool TLBB::send(connection::Basic *connection, packet::Interface *packet) {
  bool result = false;
  stream::Output &ostream = connection->ostream();
  if (!ostream.use(NET_PACKET_HEADERSIZE + packet->size())) {
    return false;
  }
  packet->set_index(connection->packet_index());
  uint32_t before_writesize = ostream.size();
  uint16_t packetid = packet->get_id();
  uint32_t packetsize = packet->size();
  uint32_t packetindex = packet->get_index();
  TLBBHead header;
  header.id = packetid;
  header.index = packetindex;
  header.size = packetsize + 3;
  tlbb_head_hton(header);
  ostream.write(reinterpret_cast<const char *>(&header), 
                sizeof(header));
  result = packet->write(ostream);
  Assert(result);
  TLBBFoot footer;
  tlbb_foot_hton(footer);
  ostream.write(reinterpret_cast<const char *>(&footer), 
                sizeof(footer));
  uint32_t after_writesize = ostream.size();
  if (packet->size() != 
      after_writesize - before_writesize - sizeof(header) - sizeof(footer)) {
    FAST_ERRORLOG(NET_MODULENAME,
                  "[net.protocol] (TLBB::send) size error,"
                  " id = %d(write: %d, should: %d)",
                  packet->get_id(),
                  after_writesize - before_writesize - 6,
                  packet->size());
    result = false;
  }
  return result;
}

packet::Interface *TLBB::read_packet(connection::Basic *connection) {
  stream::Input *istream = &connection->istream();
  TLBBHead header;
  packet::Interface *packet = nullptr;
  //if (isdisconnect()) return true; leave this to connection.
  if (!istream) return nullptr;
  if (!istream->peek((char *)&header, sizeof(header))) {
    //数据不能填充消息头
    return nullptr;
  }
  tlbb_head_ntoh(header);
  if (empty_header.mask != header.mask) {
    pf_basic::io_cerr("pack mask error: %d", header.id);
    return nullptr;
  }
  if (!NET_PACKET_FACTORYMANAGER_POINTER->
      is_valid_packet_id(header.id) &&
      !NET_PACKET_FACTORYMANAGER_POINTER->
      is_valid_dynamic_packet_id(header.id) &&
      !NET_PACKET_FACTORYMANAGER_POINTER->
      is_encrypt_packet_id(header.id)) {
    pf_basic::io_cerr("packet id error: %d", header.id);
    return nullptr;
  }
  if (!NET_PACKET_FACTORYMANAGER_POINTER->is_encrypt_packet_id(header.id) &&
      !connection->check_safe_encrypt())
    return nullptr;
  TLBBFoot footer;
  //check packet length
  if (istream->size() < 
      sizeof(header) + header.size) {
    //message not receive full
    return nullptr;
  }
  header.size -= 3;
  //check packet size
  if (!NET_PACKET_FACTORYMANAGER_POINTER->
      is_valid_dynamic_packet_id(header.id)) {
    if (header.size > 
      NET_PACKET_FACTORYMANAGER_POINTER->packet_max_size(header.id)) {
      char temp[FILENAME_MAX] = {0};
      snprintf(temp, 
               sizeof(temp) - 1, 
               "packet size error, packetid = %d", 
               header.id);
      AssertEx(false, temp);
      return nullptr;
    }
  }
  //create packet
  packet = NET_PACKET_FACTORYMANAGER_POINTER->packet_create(header.id);
  if (nullptr == packet) return nullptr;

  //packet info
  packet->set_index(static_cast<int8_t>(header.index));
  packet->set_id(header.id);
  packet->set_size(header.size);
  
  //read packet
  bool result{false};
  result = istream->skip(sizeof(header));
  result = result ? packet->read(*istream) : result;
  if (!istream->peek((char *)&footer, sizeof(footer))) {
    pf_basic::io_cerr("pack mask error: %d", header.id);
    return nullptr;
  }
  tlbb_foot_ntoh(footer);
  if (footer.mask != empty_footer.mask) {
    pf_basic::io_cerr("pack mask error: %d", header.id);
    return nullptr;
  }
  result = result ? istream->skip(sizeof(footer)) : result;
  if (false == result) {
    NET_PACKET_FACTORYMANAGER_POINTER->packet_remove(packet);
    return nullptr;
  }
  return packet;
}
