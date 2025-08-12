file(GLOB_RECURSE PIRATE_SOURCES Source/*.cpp Source/*.h)

add_executable(PirateBomb)
set_target_properties(PirateBomb PROPERTIES CXX_STANDARD 23)
target_sources(PirateBomb PRIVATE ${PIRATE_SOURCES})
target_link_libraries(PirateBomb PUBLIC NovaEngine)
target_compile_definitions(PirateBomb PRIVATE NOVA_APPLICATION_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

if(CMAKE_BUILD_TYPE MATCHES Release OR CMAKE_BUILD_TYPE MATCHES MinSizeRel)
    set_target_properties(PirateBomb PROPERTIES WIN32_EXECUTABLE TRUE)
endif ()