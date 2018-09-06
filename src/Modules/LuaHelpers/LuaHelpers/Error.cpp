#include "Error.h"

namespace LuaH {
    void Error::FailedToCreate(lua_State *L, const char *className) {
        luaL_error(L, "Failed to create %s instance.", className);
    }

    void Error::FailedToCreate(lua_State *L, const char *className, const std::exception &ex) {
        Error::FailedToCreate(L, className, ex.what());
    }

    void Error::FailedToCreate(lua_State *L, const char *className, const char *description) {
        luaL_error(L, "Failed to create %s instance: %s.", description);
    }

    void Error::FailedPropertySetter(lua_State *L, const char *className, int propKeyIdx) {
        luaL_error(L, "Can't access %s.%s", className, lua_tostring(L, propKeyIdx));
    }
}