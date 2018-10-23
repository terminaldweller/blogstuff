# C Struct to Lua table

## Overview
For this tutorial we'll change a C struct into a Lua table. The structure we'll be using won't be the simplest structure you'll come across in the wild so hopefully the tutorial will do more than just cover the basics.<br/>
We'll add the structures as `lightuserdata` and not as `userdata`. Because of that, we'll have to manage the memory ourselves. This is a tutorial so doing more is better. Complexity is good when you're learning something but bad for actual production code.<br/>
Disclaimer: 
* I'm not a Lua expert and this turotial is not supposed to be a full dive into lua tables, metatables and their implementation or behavior. The tutorial is meant as an entry point into implementing custom Lua tables.<br/>
* The C code presented does not necessarily follow best practices.<br/>

### Yet Another One?
There are already a couple of tutorials on this, yes, but the ones I managed to find were all targeting older versions of lua and as the Lua devs have clearly stated, different Lua version are really different.<br/>
This tutorial will be targeting Lua 5.3.<br/>
We'll also be using a not-so-simple structure to turn into a Lua table so there is some added value there as well.<br/>

### What you'll need
* A working C compiler(I'll be using clang)
* Lua 5.3 - You can add a copy of Lua to your working directory and experiment on that or just use your system's Lua 5.3 though for educational purposes I recommend going with Lua org's advice and adding a copy of Lua to your code base. The shiny pre-built package you pull from a repo has some work gone into it.<br/>
* Make

## C Structs
First let's take a look at the C structure we'll be using.<br/>
The primary structure is called `a_t` which has, inside it, two more structures `b_t` and `c_t`:

```c
typedef struct {
  uint64_t a_int;
  double a_float;
  char* a_string;
  b_t* a_p;
  c_t** a_pp;
} a_t;
```
```c
typedef struct {
  uint32_t b_int;
  double b_float;
} b_t;
```
```c
typedef struct {
  char* c_string;
  uint32_t c_int;
} c_t;
```
The structures are purely artificial.<br/>

## First Step: Lua Types
First let's take a look at `a_t` and decide how we want to do this.<br/>
`a_t` has five members:<br/>
* `a_int` which in Lua we can turn into an `integer`.
* `a_float` which we can turn into a `number`.
* `a_string` which will be a Lua `string`.
* `a_p` which is a pointer to another structure. As previously stated, we will turn this into a `lightuserdata`.<br/>
* `a_pp` which is a double pointer. We will turn this into a table of `lightuserdata`. We'll use numeric keys for the table.<br/>

## Second Step: Helper Functions
Now let's think about what we need to do. First we need to think about how we will be using our structure. For this example we will go with a pointer, i.e., our library code will get a pointer to the structure so we need to turn the table into `lightuserdata`.<br/>
Next, we want to be able to push and pop our new table from the Lua stack.<br/>
We can also use Lua's type check to make sure our library code complains when someone passes a bad type.<br/>
We will also add functions for pushing the structure arguments onto the stack, a fucntion that acts as our constructor for our new table(more on that later) and getter and setter methods to access our C structures fields.

Let's start:
First we will write a function that checks the type and returns the C structure:<br/>
```c
static a_t* pop_a_t(lua_State* ls, int index)
```
The body looks like this:<br/>
```c
a_t* dummy;
luaL_checktype(ls, index, LUA_TUSERDATA);
dummy = luaL_checkudata(ls, index, "a_t");
if (!dummy) printf("error:bad type, expected a_t\n");
return dummy;
```
We first check to see if the index we are getting is actually a userdata type and then check the type of the userdata we got to make sure we got the right userdata type. We check the type of the userdata by checking its metatable. We will get into that later.<br/>
This amounts to our "pop" functionality for our new type.<br/>
Now let's write a "push":<br/>
The function will look like this:<br/>
```c
a_t* dummy push_a_t(lua_State* ls, ) {
  if (!lua_checkstack(ls, 1)) {
    printf("o woe is me. no more room in hell...I mean stack.\n");return NULL;
    a_t* dummy = lua_newuserdata(ls, sizeof(a_t));
    luaL_getmetatable(ls, "a_t");
    lua_setmetatable(ls, -2);
  }
}
```
notice that we reserve new memory here using `lua_newuserdata` instead of `malloc` or what have you. This way we leave it up to Lua to handle the GC. Also note that when we push a new userdata, we also ask for its metatable and then set it for the new `userdata`. We will later register a metatable so that our code above can actually work.<br/>

Next we will write a function that pushes the fields of the structure onto the stack:<br/>
```c
int a_t_push_args(lua_State* ls, a_t* a) {
  if (!lua_checkstack(ls, 5)) {
    printf("welp. lua doesn't love you today so no more stack space for you\n");
    return 0;
  }
  lua_pushinteger(ls, a->a_int);
  lua_pushnumber(ls, a->a_float);
  lua_pushstring(ls, a->a_string);
  lua_push_b_t(ls, a->a_p);
  lua_pushlightuserdata(ls, a->a_pp);
  return 5;
}
```
Notice that we are returning 5, since our new next function which is the new function expects to see the 5 fields on top of the stack.<br/>

Next up is our new function:<br/>
```c
int new_a_t(lua_State* ls) {
  if (!lua_checkstack(ls, 6)) {
    printf("today isnt your day, is it?no more room on top of stack\n");
    return 0;
  }
  a_t* dummy = push_a_t(ls); // stack position -1 is now our pointer to a_t
  dummy->a_int = lua_tointeger(ls, -6);
  dummy->a_float = lua_tonumber(ls, -5);
  dummy->a_string = lua_tostring(ls, -4);
  dummy->a_p = lua_touserdata(ls, -3);
  dummy->a_pp = lua_touserdata(ls, -2);
  return 1;
}
```
We just push a `a_t` on top of stack and then populate the fields with the values already on top of stack.<br/>

## Third Step: Setters and Getters
Now lets move onto writing our setter and getter function.<br/>
For the non-userdata types its fairly straightforward:<br/>

```c
static int getter_a_float(lua_State* ls) {
  a_t* dummy = pop_a_t(ls, -1);
  lua_pushnumber(ls, dummy->a_number);
  return 1;
}

static int getter_a_string(lua_State* ls) {
  a_t* dummy = pop_a_t(ls, -1);
  lua_pushstring(ls, dummy->a_string);
  return 1;
}
```
As for the setters:<br/>

```c
static int setter_a_int(lua_State* ls) {
  a_t* dummy = pop_a_t(ls, 1);
  dummy->a_int = lua_checkinteger(ls, 2);
  return 1;
}
```

Now for the 4th and 5th fields:<br/>
```c
static int getter_a_p(lua_State *ls) {
  a_t* dummy = pop_a_t(ls, 1);
  lua_pushuserdata(ls, 2);
  luaL_getmetatable(ls, "b_t");
  lua_setmetatable(ls, -2);
  return 1;
}
```
For the  sake of laziness, let's assume `a_t->a_int` denotes the number of entries in `a_t->a_pp`.<br/>
```c
static int getter_a_pp(lua_State* ls) {
  a_t* dummy = pop_a_t(ls, 1);
  lua_pop(ls, -1);
  if (!lua_checkstack(ls, 3)) {
    printf("sacrifice a keyboard to the moon gods or something... couldnt grow stack.\n");
    return 0;
  }
  lua_newtable(ls);
  for (uint64_t i = 0; i < dummy->a_int; ++i) {
    lua_pushinteger(ls, i + 1);
    if (dummy->a_pp[i] != NULL) {
      c_t_push_args(ls, dummy->entries[i]);
    } else {
      lua_pop(ls, 1);
      continue;
    }
    new_c_t(ls);
    lua_settable(ls, -3);
  }
  return 1;
}
```
Notice that here we assume that we have done all the previous steps for `b_t` and `c_t`.<br/>

And for the setters:<br/>
```c
static int setter_a_int(lua_State *ls) {
  a_t* dummy = pop_a_t(ls, 1);
  dummy->a_int = luaL_checkinteger(ls, 2);
  lua_settop(ls, 1);
  return 1;
}
```
We do the same thing for `a_float` and `a_string`.<br/>
We will leave `a_p` as an excercise to the reader and just implement `a_p`:<br/>

```c
static int setter_a_pp(lua_State* ls) {
  a_t* dummy = pop_a_t(ls, 1);
  if (!lua_checkstack(ls, 3)) {
    printf("is it a curse or something? couldnt grow stack.\n");
    return 0;
  }
  int table_length = lua_rawlen(ls, 2);
  for (int i = 1; i <= table_length; ++i) {
    lua_rawgeti(ls, 2, i);
    dummy->a_pp[i - 1] = luaL_checkudata(ls, -1, "c_t");
    lua_pop(ls, 1);
  }
  lua_settop(ls, 1);
  return 1;
}
```

We are all done with the functions we needed for our new table. Now we need to make that metatable we kept using and the register it with lua.<br/>

# Fourth Step: Metatable
First, if you haven't already take a look at the chapter on metatable and metamethods on pil [here](https://www.lua.org/pil/13.html).<br/>
```c
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
```
We just list the functions we want to be accessible inside Lua code.<br/>
Please note that such functions will have to have a certain type, i.e they all should look like this:<br/>
```c
int (*myluafunc)(lua_State* ls);
```

It's a good idea to take a look at the metatable events that Lua 5.3 supports [here](http://lua-users.org/wiki/MetatableEvents) if you haven't already.<br/>

Now we move on to registering our metatable with Lua:<br/>
```c
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
```
Please note that we are registering the metatable as a global. It is generally not recommended to do so.Why you ask?<br/>
Adding a new enrty to the global table in Lua means you are already reserving that keyword, so if another library also needs that key, you are going to have lots of fun.<br/>
Entries in the global table will require Lua to look things up in the global table so it slows things down a bit, though whether the slow-down is signifacant enough really depends on you and your requirements.<br/>

Now we are almost done with our new table but there is one thing remaining and that is our table doesn't have a cozy constructor(warning:cozy constructors are not a thing).<br/>
We can use our `new` function as a constructor, we have registered it with our metatable, but it requires you to pass all the arguments at the time of construction. Sometimes it's convinient to hold off on passing all or some of the args at construction time, mostly because you are writing a library and your power users will do all sorts of unconventional and crazy/creative things with your library.<br/>

Remember metatable events?<br/>
That's what we'll use.<br/>
Lua metatables support something called metatable events. Eeach event has a string key and the value is whatever you put as the value.<br/>
The values are used whenver that event happens. Some the events are:
* `__call`
* `__pairs`
* `__sub`
* `__add`
* `__gc`
The events help us customize how our new table behaves.<br/>

For a constructor, we could use the `__call` event.<br/>
That means when someone calls are metatable in Lua, like this:<br/>
```lua
local a = a_t()
```
`a` will become a new instance of our table.<br/>
We can add a value for our metatable's `__call` key from either Lua or C. Since we are talking about Lua and havent almost written anything in Lua, let's do it in Lua:<br/>
```lua
setmetatable(a_t, {__call =
	function(self, arg1, arg2, arg3, arg4, arg5)
		local t = self.new(arg1, arg2, arg3, arg4, arg5)
		return t
	end
	}
)
```
We use our `new` method which we previously registered for our metatable. Note that Lua will pass `nil` for if we don't provide any. That's how our cozy constructor works.<br/>
Doing the same thing in C is left to the reader as an excercise.<br/>

## Final Words
The tutorial's goal is to show you one way of doing the task and not necessarily the best way of doing it. Besides, depending on your situation, you might want to do things differently so by no means is this tutorial enough. It's an entry level tutorial.<br/>
Any feedback, suggestions and/or fixes to the tutorial is much appreciated.<br/>

## Shameless Plug
I needed to turn a C struct into a lua table for an application I'm working [on](https://github.com/bloodstalker/mutator/tree/master/bruiser). Further down the line, I needed to the same for a lot more C structs with the possibility of me having to do the same for a lot more C structs. I just couldn't bring myself to do it manually for that many C structs so I decided to work on a code generator that does that for me. The result is [luatablegen](https://github.com/bloodstalker/luatablegen).<br/>
`luatablegen` is a simple script that takes the description of your C structures in an XML file and generates the C code for your new tables and metatables. It does everything we did by hand automatically for us.<br/>
`lautablegen` is in its early stages, so any feedback or help will be appreciated.<br/>
