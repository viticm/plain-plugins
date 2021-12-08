#include "pf/basic/md5.h"
#include "pf/basic/base64.h"
#include "pf/plugin/lua/tool.h"

#ifndef lua_tostringex
#define lua_tostringex(L,n) (lua_isstring((L),(n)) ? lua_tostring((L),(n)) : "")
#endif

int32_t md5(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 1);
  std::string str = lua_tostringex(L, 1);
  pf_basic::MD5 md5_str(str);
  lua_pushstring(L, md5_str.md5().c_str());
  return 1;
}

typedef void(split_callback)(void *ctx, const char *s, size_t len);

static void 
do_split(const char *source, 
         size_t len, 
         const char *sp, 
         split_callback call, 
         void *ctx) {
  if (len != 0) {
    if (!(*sp)) {
      call(ctx, source, len);
    } else {
      const char *src = source;
      const char *end = strstr(src, sp);
      while (end && *end){
        call(ctx, src, end - src);
        src = ++end;
        end = strstr(src, sp);
      }
      call(ctx, src, len - (src - source));
    }
  }
}

struct luasplit_ctx{
  lua_State *L;
  int index;
  int opt;
};

static void 
luasplit2table(void *ctx, const char *src, size_t len) {
  struct luasplit_ctx *lctx = (struct luasplit_ctx *)ctx;
  if (lctx->opt == 0 || len > 0){
    lua_pushlstring(lctx->L, src,len);
    lua_rawseti(lctx->L, -2, ++lctx->index);
  }
}

static void 
luasplit2stack(void *ctx,const char *src, size_t len) {
  struct luasplit_ctx *lctx = (struct luasplit_ctx *)ctx;
  if (lctx->opt == 0 || len > 0) {
    lua_pushlstring(lctx->L, src, len);
    ++lctx->index;
  }
}

int32_t split2table(lua_State *L) {
  size_t len;
  const char *src = luaL_checklstring(L, 1, &len);
  const char *sp = luaL_optstring(L, 2, " ");
  static const char * const optname[] = {"a", "i", NULL};
  int op = luaL_checkoption(L, 3, "a", optname);
  struct luasplit_ctx lctx = {L, 0, op};
  lua_newtable(L);
  do_split(src, len, sp, luasplit2table, &lctx);
  return 1;
}

int32_t split2stack(lua_State *L) {
  size_t len;
  const char *src = luaL_checklstring(L, 1, &len);
  const char *sp = luaL_optstring(L, 2, " ");
  static const char * const optname[] = {"a", "i", NULL};
  int op = luaL_checkoption(L, 3, "a", optname);
  struct luasplit_ctx lctx = {L, 0, op};
  do_split(src, len, sp, luasplit2stack, &lctx);
  return lctx.index;
}

#define NUM_MAX (3)
#define BUF_SIZE_MAX (15 * 1024)

static int32_t 
v2str(lua_State *L, luaL_Buffer *buf, int32_t index, int32_t level);

//Lua table to string.
static int32_t 
t2str(lua_State *L, luaL_Buffer *buf, int32_t index, int32_t level) {
  if (--level < 0) {
    lua_pushfstring(buf->L, "%p(table)", lua_topointer(L, index));
    luaL_addvalue(buf);
  } else {
    int32_t count = 0;
    lua_pushfstring(buf->L, "%p{", lua_topointer(L, index));
    luaL_addvalue(buf);

    luaL_checktype(L, index, LUA_TTABLE);
    lua_pushnil(L);
    while (lua_next(L, index - 1)) {
      v2str(L, buf, -2, level);
      luaL_addstring(buf, "=");
      v2str(L, buf, -1, level);
      luaL_addstring(buf, ",");
      lua_pop(L, 1);
      if (count >= NUM_MAX) {
        luaL_addstring(buf, "...,");
        lua_pop(L, 1);
        break;
      }
    }
    luaL_addstring(buf, "}");
  }
  return 1;
}

//Lua function to string.
static int32_t f2str(lua_State *L, luaL_Buffer *buf, int32_t index) {
  luaL_checktype(L, index, LUA_TFUNCTION);
  if (lua_iscfunction(L, index)) {
    lua_pushfstring(buf->L, "%p(cfunc)", lua_topointer(L, index));
  } else {
    lua_Debug ar;
    lua_pushvalue(L, index);
    lua_getinfo(L, ">Sln", &ar);
    lua_pushfstring(buf->L,
                    "%p(lfunc@%s:%d)",
                    lua_topointer(L, index),
                    ar.short_src,
                    ar.linedefined);
  }
  return 1;
}

//All lua value to string.
static int32_t
v2str(lua_State *L, luaL_Buffer *buf, int32_t index, int32_t level) {
  int32_t type = lua_type(L, index);
  const char *tp_name = lua_typename(L, type);
  switch (type) {
    case LUA_TSTRING:
      {
        lua_pushfstring(buf->L, "%s(%s)", lua_tostring(L, index), tp_name);
        luaL_addvalue(buf);
      }
      break;
    case LUA_TNUMBER:
      {
        lua_pushfstring(buf->L, "%f(%s)", lua_tonumber(L, index), tp_name);
        luaL_addvalue(buf);
      }
      break;
    case LUA_TTABLE:
      {
        t2str(L, buf, index, level);
      }
      break;
    case LUA_TFUNCTION:
      {
        f2str(L, buf, index);
      }
      break;
    case LUA_TTHREAD:
    case LUA_TUSERDATA:
    case LUA_TLIGHTUSERDATA:
      {
        lua_pushfstring(buf->L, "%p(%s)", lua_topointer(L, index), tp_name);
      }
      break;
    case LUA_TBOOLEAN:
      {
        lua_pushfstring(buf->L, "%s(%s)",
                        lua_toboolean(L, index) ? "true" : "false",
                        tp_name);
      }
      break;
    case LUA_TNIL:
      {
        luaL_addstring(buf, "nil");
      }
      break;
    default:
      break;
  }
  return 0;
}

int32_t traceback(lua_State *L) {
  lua_Debug ar;
  int32_t level = 1;
  int32_t index = 0;
  const char *msg = NULL;

  luaL_Buffer buf;
  luaL_buffinit(L, &buf);

  luaL_addstring(&buf, "\n------------------------------------------------\n");
  msg = lua_tostring(L, 1);
  msg = msg ? msg : "unkonw";
  luaL_addstring(&buf, "error: ");
  luaL_addstring(&buf, msg);
  luaL_addstring(&buf, "\n");
  lua_settop(L, 0);

  while (lua_getstack(L, level++, &ar)) {
    if (level >= 20 ||
        (buf.size >= BUF_SIZE_MAX && (buf.size - buf.n < 1024))) {
      luaL_addstring(&buf, "!!!!!! ============= msg size too large");
      break;
    }
    int32_t i = 1;
    const char *name = NULL;
    lua_getinfo(L, "Slnu", &ar);
    name = ar.name ? ar.name : "";
    if ('C' == *ar.what && 
        (0 == strcmp(name, "xpcall") || 0 == strcmp(name, "pcall"))) {
      break;
    }
    lua_pushfstring(buf.L, 
                    "#%d %s %s %s at %s:%d\n", 
                    ++index, 
                    ar.what ? ar.what : "",
                    ar.namewhat ? ar.namewhat : "",
                    ar.name ? ar.name : "",
                    ar.short_src,
                    ar.currentline);
    luaL_addvalue(&buf);

    while ((name = lua_getlocal(L, &ar, i++))) {
      if ('(' == name[0]) {
        lua_pop(L, 1);
        continue;
      }
      lua_pushfstring(buf.L, "  local %s=", name);
      luaL_addvalue(&buf);
      v2str(L, &buf, -1, 0);
      luaL_addstring(&buf, "\n");
      lua_pop(L, 1);
    }
  }
  luaL_addstring(&buf, "------------------------------------------------\n");
  luaL_pushresult(&buf);
  return 1;
}

#undef NUM_MAX
#undef BUF_SIZE_MAX

int32_t base64_encode(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 1);
  std::string str = lua_tostringex(L, 1);
  auto r = pf_basic::base64_encode(
      (const unsigned char *)str.c_str(), str.size()) ; 
  lua_pushstring(L, r.c_str());
  return 1;
}

int32_t base64_decode(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 1);
  std::string str = lua_tostringex(L, 1);
  auto r = pf_basic::base64_decode(str) ; 
  lua_pushstring(L, r.c_str());
  return 1;
}
