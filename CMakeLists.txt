# Build for LuaBind
# @author Natesh Narain

cmake_minimum_required(VERSION 2.8)

project(luabind)

include(ExternalProject)

set(BUILD_DIR "build")
set(EXTERNALS_DIR "${BUILD_DIR}/externals")
set(BINARY_DIR "${BUILD_DIR}/bin")

set(LUA_PATH "${CMAKE_BINARY_DIR}/${EXTERNALS_DIR}/lua-core")

# download lua 5.2 from repo
ExternalProject_Add(
	lua-core

	GIT_REPOSITORY "https://github.com/nnarain/lua-core.git"
	GIT_TAG "master"

	SOURCE_DIR "${EXTERNALS_DIR}/lua-core"
	BINARY_DIR ${BINARY_DIR}

	UPDATE_COMMAND  ""
	PATCH_COMMAND   ""
	INSTALL_COMMAND ""
)

message("-- lua-core path: ${LUA_PATH}")

find_package(Boost REQUIRED)

include_directories(
	${CMAKE_SOURCE_DIR} # luabind

	${LUA_PATH}/include

	${Boost_INCLUDE_DIRS}
)

file(GLOB_RECURSE LUABIND_SRC "src/*.cpp")

if(NOT BUILD_TYPE)
	set(BUILD_TYPE SHARED)
endif(NOT BUILD_TYPE)

if(BUILD_TYPE STREQUAL "SHARED")
	add_definitions(-DLUABIND_DYNAMIC_LINK)
endif(BUILD_TYPE STREQUAL "SHARED")

# build luabind
add_library(${PROJECT_NAME} ${BUILD_TYPE}
	${LUABIND_SRC}
)

link_directories("${CMAKE_BINARY_DIR}/${BINARY_DIR}")

target_link_libraries(${PROJECT_NAME}
	${Boost_LIBRARIES}
	#lua
)

# install
install(
	TARGETS
		${PROJECT_NAME}

	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

file(GLOB LUABIND_HEADERS "luabind/*.hpp")
file(GLOB LUABIND_DETIAL_HEADERS "luabind/detail/*.hpp")

install(
	FILES
		${LUABIND_HEADERS}
	DESTINATION
		include/luabind
)

install(
	FILES
		${LUABIND_DETIAL_HEADERS}
	DESTINATION
		include/luabind/detail
)