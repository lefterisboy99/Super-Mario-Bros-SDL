include_directories(${PROJECT_SOURCE_DIR}/Engine/include/ ../UnitTest1/src/)

add_executable(GenerateGridMap src/main.cpp ../UnitTest1/src/my_app.cpp)
target_link_libraries(GenerateGridMap Engine ${SDL_LIB_DIR}/SDL2main.lib)

set_target_properties(GenerateGridMap
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# add_custom_command(TARGET GenerateGridMap POST_BUILD
# 	COMMAND ${CMAKE_COMMAND} -E copy_if_different
# 	"${SDL_LIB_DIR}/SDL2.dll"
# 	"${RUNTIME_OUTPUT_DIRECTORY}/SDL2.dll")
message("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/SDL2.dll")

get_target_property(TG_DIR3 GenerateGridMap RUNTIME_OUTPUT_DIRECTORY)

set(SDL2_DLL_LOCATION3 "${TG_DIR3}/SDL2.dll")

add_custom_command(
	OUTPUT ${SDL2_DLL_LOCATION3}
	COMMAND ${CMAKE_COMMAND} -E copy "${SDL_LIB_DIR}/SDL2.dll" ${SDL2_DLL_LOCATION3})

add_custom_target(SDL_DLL3 DEPENDS ${SDL2_DLL_LOCATION3})

add_dependencies(GenerateGridMap SDL_DLL3)
