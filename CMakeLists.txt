# Build for LuaBind
# @author Natesh Narain

cmake_minimum_required(VERSION 2.8)

project(luabind)

find_package(Boost REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR} # luabind

    ${LUA_INCLUDE_DIRS}

    ${Boost_INCLUDE_DIRS}
)

file(GLOB_RECURSE LUABIND_SRC "src/*.cpp")

# build luabind
add_library(${PROJECT_NAME} STATIC
    ${LUABIND_SRC}
)

target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES}
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