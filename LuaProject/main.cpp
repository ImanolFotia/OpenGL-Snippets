#include <iostream>

#include <lua.hpp>

using namespace std;

int main()
{
    const char* var;
    lua_State* lua = luaL_newstate();
    luaL_openlibs(lua);

    luaL_dofile(lua, "test.lua");

    cout << lua_getglobal(lua, "func") << endl;
    lua_pushnumber(lua, 2);

    if(lua_pcall(lua, 1, 1, 0) != 0)
        cout << "Error calling the function" << endl;

    cout << lua_tointeger(lua, -1) << endl;

    lua_close(lua);
    return 0;
}
