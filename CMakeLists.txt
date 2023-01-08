cmake_minimum_required(VERSION 3.14)

project(Atom-Loader)

include_directories("${CMAKE_SOURCE_DIR}/Atom/inc")
add_subdirectory("${CMAKE_SOURCE_DIR}/Atom")

add_subdirectory("${CMAKE_SOURCE_DIR}/External/Assimp/")

target_include_directories(${PROJECT_NAME} PUBLIC ${ASSIMP_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} assimp)

