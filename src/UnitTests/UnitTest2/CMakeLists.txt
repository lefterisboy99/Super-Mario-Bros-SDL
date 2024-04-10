include_directories(${PROJECT_SOURCE_DIR}/Engine/include/ ../UnitTest1/src/)

add_executable(UnitTest2 src/main.cpp ../UnitTest1/src/my_app.cpp)
target_link_libraries(UnitTest2 Engine ${SDL_LIB_DIR}/SDL2main.lib)

set_target_properties(UnitTest2
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# add_custom_command(TARGET UnitTest2 POST_BUILD
# 	COMMAND ${CMAKE_COMMAND} -E copy_if_different
# 	"${SDL_LIB_DIR}/SDL2.dll"
# 	"${RUNTIME_OUTPUT_DIRECTORY}/SDL2.dll")
message("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/SDL2.dll")

get_target_property(TG_DIR2 UnitTest2 RUNTIME_OUTPUT_DIRECTORY)

set(SDL2_DLL_LOCATION2 "${TG_DIR2}/SDL2.dll")

add_custom_command(
	OUTPUT ${SDL2_DLL_LOCATION2}
	COMMAND ${CMAKE_COMMAND} -E copy "${SDL_LIB_DIR}/SDL2.dll" ${SDL2_DLL_LOCATION2})

add_custom_target(SDL_DLL2 DEPENDS ${SDL2_DLL_LOCATION2})

add_dependencies(UnitTest2 SDL_DLL2)
