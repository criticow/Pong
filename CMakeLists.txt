cmake_minimum_required(VERSION 3.31.1)

set(CMAKE_CXX_STANDARD 20)

project(pongus)

file(GLOB_RECURSE SOURCE "src/*.cpp")

set(DDENGINE_BUILD_TESTS OFF)
set(DDENGINE_BUILD_EXAMPLES OFF)

add_executable(${PROJECT_NAME} ${SOURCE} resources.rc)

add_subdirectory(deps/ddengine)

target_link_libraries(${PROJECT_NAME} PRIVATE ddengine)

set(DATA_DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/data)

# postbuild command to copy the data folder
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/data ${DATA_DESTINATION}
)

add_custom_target(
  copy_data_folder ALL
  DEPENDS ${DATA_DESTINATION}
)

# Check if it's a release build
if (CMAKE_BUILD_TYPE STREQUAL "Release")
  target_compile_definitions(${PROJECT_NAME} PRIVATE -DNDEBUG)
  set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
endif()
