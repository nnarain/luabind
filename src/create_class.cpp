// Copyright (c) 2003 Daniel Wallin and Arvid Norberg

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#define LUABIND_NO_HEADERS_ONLY

#include <luabind/luabind.hpp>

using namespace luabind::detail;

int luabind::detail::create_class::stage2(lua_State* L)
{
	class_rep* crep = static_cast<class_rep*>(lua_touserdata(L, lua_upvalueindex(1)));
	assert((crep != 0) && "internal error, please report");
	assert((is_class_rep(L, lua_upvalueindex(1))) && "internal error, please report");

#ifndef LUABIND_NO_ERROR_CHECKING

	if (!is_class_rep(L, 1))
	{
		lua_pushstring(L, "expected class to derive from or a newline");
		lua_error(L);
	}

#endif

	class_rep* base = static_cast<class_rep*>(lua_touserdata(L, 1));
	class_rep::base_info binfo;

	binfo.pointer_offset = 0;
	binfo.base = base;
	crep->add_base_class(binfo);

	if (base->get_class_type() == class_rep::lua_class)
	{
		// copy base class members

		detail::getref(L, crep->table_ref());
		detail::getref(L, base->table_ref());
		lua_pushnil(L);

		while (lua_next(L, -2))
		{
			lua_pushstring(L, "__init");
			if (lua_equal(L, -1, -3))
			{
				lua_pop(L, 2);
				continue;
			}

			lua_pushstring(L, "__finalize");
			if (lua_equal(L, -1, -3))
			{
				lua_pop(L, 2);
				continue;
			}

			lua_pushvalue(L, -2); // copy key
			lua_insert(L, -2);
			lua_settable(L, -5);
		}
	}

	crep->set_type(base->type());

	return 0;
}

int luabind::detail::create_class::stage1(lua_State* L)
{

#ifndef LUABIND_NO_ERROR_CHECKING

	if (lua_gettop(L) != 1 || lua_type(L, 1) != LUA_TSTRING || lua_isnumber(L, 1))
	{
		lua_pushstring(L, "invalid construct, expected class name");
		lua_error(L);
	}

#endif

	const char* name = lua_tostring(L, 1);

	void* c = lua_newuserdata(L, sizeof(class_rep));
	new(c) class_rep(L, name);

	// make the class globally available
	lua_pushstring(L, name);
	lua_pushvalue(L, -2);
	lua_settable(L, LUA_GLOBALSINDEX);

	// also add it to the closure as return value
	lua_pushcclosure(L, &stage2, 1);

	return 1;
}
