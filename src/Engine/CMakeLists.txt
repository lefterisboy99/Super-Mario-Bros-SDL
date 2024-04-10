message("Enter Directory ENGINE")

include_directories(include/)

add_library(Engine STATIC src/game.cpp src/app.cpp src/event.cpp src/input_system.cpp src/rendition_system.cpp src/csv_importer.cpp src/texture_system.cpp src/tilemap_system.cpp src/gridmap.cpp src/spritesheet.cpp src/layer.cpp src/common.cpp src/sprite_system.cpp src/gravity_handler.cpp src/audio_system.cpp)


message ("SDL link Libs: " ${SDL_LIB_DIR}/SDL2 ${SDL_LIB_DIR}/SDL2main)
target_include_directories(Engine PRIVATE ${SDL_MIXER_INCLUDE})
if (WIN32)
	target_link_libraries(Engine ${SDL_LIB_DIR}/SDL2.lib shell32 opengl32 ${SDL_MIXER_LIB}/SDL2_mixer.lib)
elseif (UNIX)
	target_link_libraries(Engine ${SDL_LIB_DIR}/SDL2 ${SDL_LIB_DIR}/SDL2main GL)
endif (WIN32)

# Enable highest warning level, disable annoying warnings
if(MSVC)
	set(CMAKE_C_FLAGS "/W4")
	target_compile_options(Engine PRIVATE /WX /wd4204 /wd4201 /wd4221)
else()
	target_compile_options(Engine PRIVATE -Wall -Wextra -Werror -Wno-missing-braces -Wno-parentheses -Wno-unused-value -Wno-unused-function -Wno-unused-variable)
endif()

set_target_properties(Engine
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

message("-- Current compiler flags CMAKE_C_FLAGS are: ${CMAKE_C_FLAGS}")