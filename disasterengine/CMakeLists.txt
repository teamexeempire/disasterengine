cmake_minimum_required(VERSION 3.8)
project(disasterengine)

add_executable(disasterengine 
    "Main.cc" 
    "MainLoop.cc" 
    "Input.cc"
    "res/Resources.cc"
    
    "video/Video.cc" 
    "video/GLExt.cc" 
    "video/Sprite.cc" 
    "video/Shader.cc" 
    "video/Render.cc" 
    
    "audio/Audio.cc" 
    "audio/Sample.cc" 
    "audio/Music.cc" 
    
    "lua/Lua.cc" 
    "lua/LuaExtCore.cc"
    "lua/LuaExtMath.cc"
    "lua/LuaExtVideo.cc"
    "lua/LuaExtAudio.cc" 
    "lua/LuaExt.cc" 
)
add_dependencies(disasterengine modpack)

find_package(OpenGL REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_package(spdlog CONFIG REQUIRED)

target_include_directories(disasterengine PRIVATE ${LUA_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIRS})

set_target_properties(disasterengine PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(disasterengine PRIVATE spdlog::spdlog_header_only)
target_link_libraries(disasterengine PRIVATE ${LUA_LIBRARIES})
target_link_libraries(disasterengine PRIVATE $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>)
target_link_libraries(disasterengine PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)
target_link_libraries(disasterengine PRIVATE $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main> $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)
target_link_libraries(disasterengine PRIVATE ${OPENGL_LIBRARIES})


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET disasterengine PROPERTY CXX_STANDARD 20)
endif()

install(
  TARGETS disasterengine
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

add_custom_command(TARGET disasterengine POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:disasterengine> $<TARGET_RUNTIME_DLLS:disasterengine>
  COMMAND_EXPAND_LISTS
  COMMENT "Copying dlls..."
  VERBATIM
)

add_custom_target(
    my_custom_target_that_always_runs ALL
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/balls"
)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/balls"
  COMMAND modpack "${CMAKE_SOURCE_DIR}/gamedata" "gamedata.mpack"
  COMMAND_EXPAND_LISTS
  DEPENDS "${CMAKE_SOURCE_DIR}/gamedata/"
  COMMENT "Building assets..."
  VERBATIM
)