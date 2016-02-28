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
	lua

	GIT_REPOSITORY "https://github.com/nnarain/lua-core.git"
	GIT_TAG "develop"

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

# build luabind
add_library(${PROJECT_NAME}
	${LUABIND_SRC}
)

target_link_libraries(${PROJECT_NAME}
	${Boost_LIBRARIES}
)

