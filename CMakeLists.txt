
cmake_minimum_required(VERSION 2.8)
project(VoxelGame)

# Add deps subdirectory
add_subdirectory(dependencies)

# Get Include Directories
set (INCLUDE_DIRS include
	dependencies/glfw/include/GLFW
	dependencies/glew/include/GL
	dependencies/glm
	dependencies/glm/gtc
	dependencies/glm/gtx

  dependencies/Simple-OpenGL-Image-Library/src
  dependencies/tinyobjloader

#	dependencies/assimp/include/assimp
)

include_directories(${INCLUDE_DIRS})

# Get all source files from src
aux_source_directory(src SRC_LIST)
aux_source_directory(src/entities SRC_LIST2)
aux_source_directory(src/graphics SRC_LIST3)
aux_source_directory(src/graphics/mesh SRC_LIST4)
aux_source_directory(src/graphics/texture SRC_LIST5)
aux_source_directory(src/demo SRC_LIST6)
aux_source_directory(src/util SRC_LIST7)
aux_source_directory(src/config SRC_LIST8)

# Add executable to project
add_executable(${PROJECT_NAME} 
	${SRC_LIST} ${SRC_LIST2} ${SRC_LIST3} ${SRC_LIST4} 
	${SRC_LIST5} ${SRC_LIST6} ${SRC_LIST7} ${SRC_LIST8} 
)

# Link GLFW and GLEW to main executable

# assimp

target_link_libraries(${PROJECT_NAME} libglew_static glfw ${GLFW_LIBRARIES} SOIL tinyobjloader )

# Define GLEW_STATIC for Windows
#if(MSVC)
#	add_definitions(-DGLEW_STATIC)
#endif()

add_definitions( -std=c++0x -Wextra)

