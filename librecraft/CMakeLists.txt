cmake_minimum_required(VERSION 2.8)
project(VoxelGame)

add_definitions(-std=c++14 -Wextra)

# Add deps subdirectory
add_subdirectory(dependencies)

# Get Include Directories
set (INCLUDE_DIRS 
    include
    include/GL
    include/glm
    include/glm/gtc
    include/glm/gtx
    include/ThreadPool
)

include_directories(${INCLUDE_DIRS})

aux_source_directory(src SRC_LIST)
aux_source_directory(src/entities SRC_LIST)
aux_source_directory(src/graphics SRC_LIST)
aux_source_directory(src/graphics/mesh SRC_LIST)
aux_source_directory(src/graphics/texture SRC_LIST)
aux_source_directory(src/util SRC_LIST)
aux_source_directory(src/config SRC_LIST)
aux_source_directory(src/model SRC_LIST)
aux_source_directory(src/model/world/chunk SRC_LIST)
aux_source_directory(src/model/world/terrainGen SRC_LIST)
aux_source_directory(src/model/world/entity SRC_LIST)
aux_source_directory(src/model/world/timeAndWeather SRC_LIST)
aux_source_directory(src/gui SRC_LIST)
aux_source_directory(src/gui/widget SRC_LIST)

aux_source_directory(src/demo SRC_DEMO_LIST)
aux_source_directory(src/entities SRC_DEMO_LIST)
aux_source_directory(src/graphics SRC_DEMO_LIST)
aux_source_directory(src/graphics/mesh SRC_DEMO_LIST)
aux_source_directory(src/graphics/texture SRC_DEMO_LIST)
aux_source_directory(src/util SRC_DEMO_LIST)
aux_source_directory(src/config SRC_DEMO_LIST)
aux_source_directory(src/model SRC_DEMO_LIST)
aux_source_directory(src/model/world/chunk SRC_DEMO_LIST)
aux_source_directory(src/model/world/terrainGen SRC_DEMO_LIST)
aux_source_directory(src/model/world/entity SRC_DEMO_LIST)
aux_source_directory(src/model/world/timeAndWeather SRC_DEMO_LIST)
aux_source_directory(src/gui SRC_DEMO_LIST)
aux_source_directory(src/gui/widget SRC_DEMO_LIST)

# Add executable to project
add_executable(${PROJECT_NAME} ${SRC_LIST})
add_executable(Demos ${SRC_DEMO_LIST})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    # For some weird reason, I have to link with pthread, which was not necessary before.
    target_link_libraries(${PROJECT_NAME} pthread libglew_static ${SFML_LIBRARIES})
	target_link_libraries(Demos pthread libglew_static ${SFML_LIBRARIES})
endif()


