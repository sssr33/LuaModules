#include "DataReader.h"

#include <sstream>
#include <libhelpers/HMathCP.h>

namespace LuaH {

    int DataReader::GetStackHelper<float>::GetType(lua_State *L, int idx) {
        return LUA_TNUMBER;
    }

    float DataReader::GetStackHelper<float>::ToType(lua_State *L, int idx, const char *errorName) {
        return (float)lua_tonumber(L, idx);
    }

    int DataReader::GetStackHelper<uint32_t>::GetType(lua_State *L, int idx) {
        return LUA_TNUMBER;
    }

    uint32_t DataReader::GetStackHelper<uint32_t>::ToType(lua_State *L, int idx, const char *errorName) {
        lua_Integer num = lua_tointeger(L, idx);

        if (num < H::MathCP::MinValue<lua_Integer, uint32_t>() ||
            num > H::MathCP::MaxValue<lua_Integer, uint32_t>())
        {
            std::stringstream strStream;
            strStream << errorName << "value(" << num << ") is out of bounds[" << H::MathCP::MinValue<lua_Integer, uint32_t>() << "; " << H::MathCP::MaxValue<lua_Integer, uint32_t>() << "].";
            luaL_error(L, strStream.str().c_str());
        }

        return (uint32_t)num;
    }

    int DataReader::GetStackHelper<bool>::GetType(lua_State *L, int idx) {
        return LUA_TBOOLEAN;
    }

    bool DataReader::GetStackHelper<bool>::ToType(lua_State *L, int idx, const char *errorName) {
        bool val = lua_toboolean(L, idx) != 0;
        return val;
    }
}