include_directories(${PROJECT_SOURCE_DIR}/Engine/include/)

add_executable(UnitTest1 src/main.cpp src/my_app.cpp)
target_link_libraries(UnitTest1 Engine ${SDL_LIB_DIR}/SDL2main.lib)

set_target_properties(UnitTest1
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# add_custom_command(TARGET UnitTest1 POST_BUILD
# 	COMMAND ${CMAKE_COMMAND} -E copy_if_different
# 	"${SDL_LIB_DIR}/SDL2.dll"
# 	"${RUNTIME_OUTPUT_DIRECTORY}/SDL2.dll")
message("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/SDL2.dll")

get_target_property(TG_DIR UnitTest1 RUNTIME_OUTPUT_DIRECTORY)

set(SDL2_DLL_LOCATION "${TG_DIR}/SDL2.dll")

add_custom_command(
	OUTPUT ${SDL2_DLL_LOCATION}
	COMMAND ${CMAKE_COMMAND} -E copy "${SDL_LIB_DIR}/SDL2.dll" ${SDL2_DLL_LOCATION})

add_custom_target(SDL_DLL DEPENDS ${SDL2_DLL_LOCATION})

add_dependencies(UnitTest1 SDL_DLL)
