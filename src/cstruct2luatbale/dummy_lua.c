
/******************************************************************************/
#include "./lua/lauxlib.h"
#include "./lua/lua.h"
#include "./lua/lualib.h"
#include "./lua.c"
#include <stdio.h>
#include <inttypes.h>
/******************************************************************************/
typedef struct {
  uint32_t b_int;
  double b_float;
}b_t;

typedef struct {
  char* c_string;
  uint32_t c_int;
}c_t;

typedef struct {
  uint64_t a_int;
  double a_float;
  char* a_string;
  b_t* a_p;
  c_t** a_pp;
}a_t;
/******************************************************************************/
static b_t* pop_b_t(lua_State* ls, int index) {
  b_t* dummy;
  //luaL_checktype(ls, index, LUA_TUSERDATA);
  dummy = luaL_checkudata(ls, index, "b_t");
  if (!dummy) printf("error:bad type, expected b_t\n");
  return dummy;
}

b_t* push_b_t(lua_State* ls) {
  if (!lua_checkstack(ls, 1)) {
    printf("o woe is me. no more room in hell...I mean stack...\n");return NULL;
  }
  b_t* dummy = lua_newuserdata(ls, sizeof(b_t));
  luaL_getmetatable(ls, "b_t");
  lua_setmetatable(ls, -2);
  return dummy;
}

int b_t_push_args(lua_State* ls, b_t* b) {
  if (!lua_checkstack(ls, 2)) {
    printf("welp. lua doesn't love you today so no more stack space for you\n");
    return 0;
  }
  lua_pushnumber(ls, b->b_float);
  lua_pushinteger(ls, b->b_int);
  return 2;
}

int new_b_t(lua_State* ls) {
  if (!lua_checkstack(ls, 3)) {
    printf("today isnt your day, is it?no more room on top of stack\n");
    return 0;
  }
  float b_number_float = lua_tonumber(ls, -2);
  int b_int = lua_tointeger(ls, -1);
  lua_pop(ls, 2);
  b_t* dummy = push_b_t(ls); // stack position -3 or 1 is now our pointer to a_t
  dummy->b_float = lua_tonumber(ls, -2);
  dummy->b_int = lua_tointeger(ls, -1);
  return 1;
}
static int getter_b_float(lua_State* ls) {
  b_t* dummy = pop_b_t(ls, 1);
  lua_pushnumber(ls, dummy->b_float);
  return 1;
}

static int getter_b_int(lua_State* ls) {
  b_t* dummy = pop_b_t(ls, 1);
  lua_pushinteger(ls, dummy->b_int);
  return 1;
}

static int setter_b_int(lua_State *ls) {
  b_t* dummy = pop_b_t(ls, 1);
  dummy->b_int = luaL_checkinteger(ls, 2);
  lua_settop(ls, 1);
  return 1;
}

static int setter_b_float(lua_State *ls) {
  b_t* dummy = pop_b_t(ls, 1);
  dummy->b_float = lua_tonumber(ls, 2);
  lua_settop(ls, 1);
  return 1;
}

static const luaL_Reg b_t_methods[] = {
    {"new", new_b_t},
    {"set_b_int", setter_b_int},
    {"set_b_float", setter_b_float},
    {"b_int", getter_b_int},
    {"b_float", getter_b_float},
    {0, 0}};
static const luaL_Reg b_t_meta[] = {{0, 0}};

int b_t_register(lua_State *ls) {
  lua_checkstack(ls, 4);
  lua_newtable(ls);
  luaL_setfuncs(ls, b_t_methods, 0);
  luaL_newmetatable(ls, "b_t");
  luaL_setfuncs(ls, b_t_methods, 0);
  luaL_setfuncs(ls, b_t_meta, 0);
  lua_pushliteral(ls, "__index");
  lua_pushvalue(ls, -3);
  lua_rawset(ls, -3);
  lua_pushliteral(ls, "__metatable");
  lua_pushvalue(ls, -3);
  lua_rawset(ls, -3);
  lua_setglobal(ls, "b_t");
  return 0;
}
/******************************************************************************/
static c_t* pop_c_t(lua_State* ls, int index) {
  c_t* dummy;
  //luaL_checktype(ls, index, LUA_TUSERDATA);
  dummy = luaL_checkudata(ls, index, "c_t");
  if (!dummy) printf("error:bad type, expected c_t\n");
  return dummy;
}

c_t* push_c_t(lua_State* ls) {
  if (!lua_checkstack(ls, 1)) {
    printf("o woe is me. no more room in hell...I mean stack...\n");return NULL;
  }
  c_t* dummy = lua_newuserdata(ls, sizeof(c_t));
  luaL_getmetatable(ls, "c_t");
  lua_setmetatable(ls, -2);
  return dummy;
}

int c_t_push_args(lua_State* ls, c_t* c) {
  if (!lua_checkstack(ls, 2)) {
    printf("welp. lua doesn't love you today so no more stack space for you\n");
    return 0;
  }
  lua_pushinteger(ls, c->c_int);
  lua_pushstring(ls, c->c_string);
  return 2;
}

int new_c_t(lua_State* ls) {
  if (!lua_checkstack(ls, 3)) {
    printf("today isnt your day, is it?no more room on top of stack\n");
    return 0;
  }
  // we intentionally use intermediate variables, this way if somebody passes no 
  // args to our new function(or our lua constructor), they will still get an
  // instance of our table
  int c_int = lua_tointeger(ls, -2);
  char* c_string = lua_tostring(ls, -1);
  lua_pop(ls, 2);
  c_t* dummy = push_c_t(ls); // stack position -3 or 1 is now our pointer to a_t
  dummy->c_int = c_int;
  dummy->c_string = c_string;
  return 1;
}

static int getter_c_string(lua_State* ls) {
  c_t* dummy = pop_c_t(ls, 1);
  lua_pushstring(ls, dummy->c_string);
  return 1;
}

static int getter_c_int(lua_State* ls) {
  c_t* dummy = pop_c_t(ls, 1);
  lua_pushinteger(ls, dummy->c_int);
  return 1;
}

static int setter_c_int(lua_State *ls) {
  c_t* dummy = pop_c_t(ls, 1);
  dummy->c_int = luaL_checkinteger(ls, 2);
  lua_settop(ls, 1);
  return 1;
}

static int setter_c_string(lua_State *ls) {
  c_t* dummy = pop_c_t(ls, 1);
  dummy->c_string = lua_tostring(ls, 2);
  lua_settop(ls, 1);
  return 1;
}

static const luaL_Reg c_t_methods[] = {
    {"new", new_c_t},
    {"set_c_int", setter_c_int},
    {"set_c_string", setter_c_string},
    {"c_int", getter_c_int},
    {"c_string", getter_c_string},
    {0, 0}};
static const luaL_Reg c_t_meta[] = {{0, 0}};

int c_t_register(lua_State *ls) {
  lua_checkstack(ls, 4);
  lua_newtable(ls);
  luaL_setfuncs(ls, c_t_methods, 0);
  luaL_newmetatable(ls, "c_t");
  luaL_setfuncs(ls, c_t_methods, 0);
  luaL_setfuncs(ls, c_t_meta, 0);
  lua_pushliteral(ls, "__index");
  lua_pushvalue(ls, -3);
  lua_rawset(ls, -3);
  lua_pushliteral(ls, "__metatable");
  lua_pushvalue(ls, -3);
  lua_rawset(ls, -3);
  lua_setglobal(ls, "c_t");
  return 0;
}
/******************************************************************************/
static a_t* pop_a_t(lua_State* ls, int index) {
  a_t* dummy;
  //luaL_checktype(ls, index, LUA_TUSERDATA);
  dummy = luaL_checkudata(ls, index, "a_t");
  if (!dummy) printf("error:bad type, expected a_t\n");
  return dummy;
}

a_t* push_a_t(lua_State* ls) {
  if (!lua_checkstack(ls, 1)) {
      printf("o woe is me. no more room in hell...I mean stack...\n");return NULL;
    }
  a_t* dummy = lua_newuserdata(ls, sizeof(a_t));
  luaL_getmetatable(ls, "a_t");
  lua_setmetatable(ls, -2);
  return dummy;
}

int a_t_push_args(lua_State* ls, a_t* a) {
  if (!lua_checkstack(ls, 5)) {
    printf("welp. lua doesn't love you today so no more stack space for you\n");
    return 0;
  }
  lua_pushinteger(ls, a->a_int);
  lua_pushnumber(ls, a->a_float);
  lua_pushstring(ls, a->a_string);
  push_b_t(ls);
  lua_pushlightuserdata(ls, a->a_pp);
  return 5;
}

int new_a_t(lua_State* ls) {
  if (!lua_checkstack(ls, 6)) {
    printf("today isnt your day, is it?no more room on top of stack\n");
    return 0;
  }
  int a_int = lua_tointeger(ls, -1);
  float a_float = lua_tonumber(ls, -2);
  char* a_string = lua_tostring(ls, -3);
  void* a_p = lua_touserdata(ls, -4);
  void** a_pp = lua_touserdata(ls, -5);
  lua_pop(ls, 5);
  a_t* dummy = push_a_t(ls); // stack position -6 or 1 is now our pointer to a_t
  dummy->a_int = a_int;
  dummy->a_float = a_float;
  dummy->a_string = a_string;
  dummy->a_p = a_p;
  dummy->a_pp = a_pp;
  return 1;
}

static int getter_a_float(lua_State* ls) {
  a_t* dummy = pop_a_t(ls, -1);
  lua_pushnumber(ls, dummy->a_float);
  return 1;
}

static int getter_a_string(lua_State* ls) {
  a_t* dummy = pop_a_t(ls, -1);
  lua_pushstring(ls, dummy->a_string);
  return 1;
}

static int getter_a_int(lua_State* ls) {
  a_t* dummy = pop_a_t(ls, 1);
  lua_pushinteger(ls, dummy->a_int);
  return 1;
}

static int getter_a_p(lua_State *ls) {
  a_t* dummy = pop_a_t(ls, 1);
  lua_pushlightuserdata(ls, dummy->a_p);
  luaL_getmetatable(ls, "b_t");
  lua_setmetatable(ls, -2);
  return 1;
}

static int getter_a_pp(lua_State* ls) {
  printf("stack height:%d\n", lua_gettop(ls));
  a_t* dummy = pop_a_t(ls, 1);
  lua_pop(ls, -1);
  printf("stack height:%d\n", lua_gettop(ls));
  if (!lua_checkstack(ls, 3)) {
    printf("sacrifice a keyboard to the moon gods or something... couldnt grow stack.\n");
    return 0;
  }
  lua_newtable(ls);
  for (uint64_t i = 0; i < dummy->a_int; ++i) {
    lua_pushinteger(ls, i + 1);
    if (dummy->a_pp[i] != NULL) {
      c_t_push_args(ls, dummy->a_pp[i]);
    } else {
      printf("fuck\n");
      lua_pop(ls, 1);
      continue;
    }
    new_c_t(ls);
    printf("here\n");
    printf("stack height:%d\n", lua_gettop(ls));
    lua_settable(ls, -3);
  }
  return 1;
}

static int setter_a_int(lua_State *ls) {
  a_t* dummy = pop_a_t(ls, 1);
  dummy->a_int = luaL_checkinteger(ls, 2);
  lua_settop(ls, 1);
  return 0;
}

static int setter_a_float(lua_State *ls) {
  a_t* dummy = pop_a_t(ls, 1);
  dummy->a_float = lua_tonumber(ls, 2);
  lua_settop(ls, 1);
  return 0;
}

static int setter_a_string(lua_State *ls) {
  a_t* dummy = pop_a_t(ls, 1);
  dummy->a_string = lua_tostring(ls, 2);
  lua_settop(ls, 1);
  return 0;
}

static int setter_a_p(lua_State *ls) {
  a_t* dummy = pop_a_t(ls, 1);
  dummy->a_p = luaL_checkudata(ls, 2, "b_t");
  lua_settop(ls, 1);
  return 0;
}

// notice how we assume our new table type is always at the top of the stack?
// the reason we do that is because from Lua the call looks like this:
// a:set_a_pp(pp)
// the above expression is syntactic sugar for a.set_a_pp(self, pp)
// so the first argument being passed and hence the first argument on top of
// the stack will be our table instance itself.
static int setter_a_pp(lua_State* ls) {
  a_t* dummy = pop_a_t(ls, 1);
  dummy->a_pp = malloc(sizeof(void*));
  if (!lua_checkstack(ls, 3)) {
    printf("is it a curse or something? couldnt grow stack.\n");
    return 0;
  }
  int table_length = lua_rawlen(ls, 2);
  for (int i = 1; i <= table_length; ++i) {
    lua_rawgeti(ls, 2, i);
    //luaL_getmetatable(ls, "c_t");
    //lua_setmetatable(ls, -2);
    //dummy->a_pp[i - 1] = luaL_checkudata(ls, -1, "c_t");
    dummy->a_pp[i - 1] = lua_touserdata(ls, -1);
    printf("int:%d\n", dummy->a_pp[i-1]->c_int);
    lua_pop(ls, 1);
  }
  //lua_pop(ls, 1);
  //lua_settop(ls, 1);
  return 0;
}

static const luaL_Reg a_t_methods[] = {
    {"new", new_a_t},
    {"set_a_int", setter_a_int},
    {"set_a_float", setter_a_float},
    {"set_a_string", setter_a_string},
    {"set_a_p", setter_a_p},
    {"set_a_pp", setter_a_pp},
    {"a_int", getter_a_int},
    {"a_float", getter_a_float},
    {"a_string", getter_a_string},
    {"a_p", getter_a_p},
    {"a_pp", getter_a_pp},
    {0, 0}};
static const luaL_Reg a_t_meta[] = {{0, 0}};

int a_t_register(lua_State *ls) {
  lua_checkstack(ls, 4);
  lua_newtable(ls);
  luaL_setfuncs(ls, a_t_methods, 0);
  luaL_newmetatable(ls, "a_t");
  luaL_setfuncs(ls, a_t_methods, 0);
  luaL_setfuncs(ls, a_t_meta, 0);
  lua_pushliteral(ls, "__index");
  lua_pushvalue(ls, -3);
  lua_rawset(ls, -3);
  lua_pushliteral(ls, "__metatable");
  lua_pushvalue(ls, -3);
  lua_rawset(ls, -3);
  lua_setglobal(ls, "a_t");
  return 0;
}
/******************************************************************************/
#pragma weak main
int main (int argc, char **argv) {
  int status, result;
  lua_State *ls = luaL_newstate();  /* create state */
  a_t_register(ls);
  b_t_register(ls);
  c_t_register(ls);
  if (ls == NULL) {
    l_message(argv[0], "cannot create state: not enough memory");
    return EXIT_FAILURE;
  }
  lua_pushcfunction(ls, &pmain);  /* to call 'pmain' in protected mode */
  lua_pushinteger(ls, argc);  /* 1st argument */
  lua_pushlightuserdata(ls, argv); /* 2nd argument */
  status = lua_pcall(ls, 2, 1, 0);  /* do the call */
  result = lua_toboolean(ls, -1);  /* get result */
  report(ls, status);
  lua_close(ls);
  return (result && status == LUA_OK) ? EXIT_SUCCESS : EXIT_FAILURE;
}

