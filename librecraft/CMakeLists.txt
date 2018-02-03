cmake_minimum_required(VERSION 2.8)
project(VoxelGame)

# set(SFML_ROOT "${CMAKE_SOURCE_DIR}/SFML-2.4.2")

message("### Building with ${CMAKE_CXX_COMPILER_ID}")

if (WARNINGS)
  message("### Warnings on")
  set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra")
else()
  message("### Warnings off")
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
  message("### Debug build.")
	add_definitions(-O0)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
  message("### Release build. Using -03 optimization")
  add_definitions(-O3)
else()
  message("### No build type specified.")
endif()

if (APPLICATION)
  message("### Building application")
else()
    message("### Not building application")
endif()

if (DEMO)
  message("### Building demos")
else()
  message("### Not building demos")
endif()

# Add deps subdirectory
add_subdirectory(dependencies)

# Get Include Directories
set (INCLUDE_DIRS 
  include
#  include/GL
  include/glm
  include/glm/gtc
  include/glm/gtx
  include/ThreadPool
)

include_directories(${INCLUDE_DIRS})

aux_source_directory(src SRC_LIST)

aux_source_directory(src/entities COMMON)
aux_source_directory(src/graphics COMMON)
aux_source_directory(src/graphics/mesh COMMON)
aux_source_directory(src/graphics/texture COMMON)
aux_source_directory(src/util COMMON)
aux_source_directory(src/config COMMON)
aux_source_directory(src/model COMMON)
aux_source_directory(src/model/world/chunk COMMON)
aux_source_directory(src/model/world/terrainGen COMMON)
aux_source_directory(src/model/world/entity COMMON)
aux_source_directory(src/model/world/timeAndWeather COMMON)
aux_source_directory(src/dependencies/libnoise COMMON)
aux_source_directory(src/dependencies/libnoise/module COMMON)
aux_source_directory(src/dependencies/libnoise/model COMMON)
aux_source_directory(src/dependencies/tinyXml2 COMMON)
aux_source_directory(src/gui COMMON)
aux_source_directory(src/gui/widget COMMON)

if(APPLICATION)
  aux_source_directory(src/ SRC_LIST)
  foreach(f ${COMMON})
    list(APPEND SRC_LIST ${f})
  endforeach()
endif()

if(DEMO)
  aux_source_directory(src/demo SRC_DEMO_LIST)
  foreach(f ${COMMON})
    list(APPEND SRC_DEMO_LIST ${f})
  endforeach()
endif()

# Add executable to project
if(APPLICATION)
  add_executable(${PROJECT_NAME} ${SRC_LIST})
endif()

if(DEMO)
  add_executable(Demos ${SRC_DEMO_LIST})
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics audio)

message("### SFML Inlucd dir = ${SFML_INCLUDE_DIR}")
message("### SFML Libraries = ${SFML_LIBRARIES}")

if(SFML_FOUND)
  foreach(f ${SFML_INCLUDE_DIR})
    message("SFML INC --- ${f}") 
  endforeach()
  include_directories(${SFML_INCLUDE_DIR})
  if (APPLICATION)
    target_link_libraries(${PROJECT_NAME} pthread libglew_static ${SFML_LIBRARIES})
  endif()
  if (DEMO)
    target_link_libraries(Demos pthread libglew_static ${SFML_LIBRARIES})
  endif()
endif()


