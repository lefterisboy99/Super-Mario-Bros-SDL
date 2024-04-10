include_directories(${PROJECT_SOURCE_DIR}/Engine/include/)

add_executable(Main src/main.cpp src/my_app.cpp)
target_link_libraries(Main Engine ${SDL_LIB_DIR}/SDL2main.lib)

set_target_properties(Main
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# add_custom_command(TARGET Main POST_BUILD
# 	COMMAND ${CMAKE_COMMAND} -E copy_if_different
# 	"${SDL_LIB_DIR}/SDL2.dll"
# 	"${RUNTIME_OUTPUT_DIRECTORY}/SDL2.dll")
message("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/SDL2.dll")

get_target_property(TG_DIR4 Main RUNTIME_OUTPUT_DIRECTORY)

set(SDL2_DLL_LOCATION4 "${TG_DIR4}/SDL2.dll")

add_custom_command(
	OUTPUT ${SDL2_DLL_LOCATION4}
	COMMAND ${CMAKE_COMMAND} -E copy "${SDL_LIB_DIR}/SDL2.dll" ${SDL2_DLL_LOCATION4})

add_custom_target(SDL_DLL4 DEPENDS ${SDL2_DLL_LOCATION4})

add_dependencies(Main SDL_DLL4)


add_custom_target(copy_files ALL)

add_custom_command(TARGET copy_files
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/ThirdParty/SDL/lib/x64/SDL2.dll ${TG_DIR4}/bin
	DEPENDS Main)

add_custom_command(TARGET copy_files
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/ThirdParty/SDL_Mixer/lib/x64/SDL2_mixer.dll ${TG_DIR4}/bin
	DEPENDS Main)

add_custom_command(TARGET copy_files
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/ThirdParty/SDL_Mixer/lib/x64/libmpg123-0.dll ${TG_DIR4}/bin
	DEPENDS Main)