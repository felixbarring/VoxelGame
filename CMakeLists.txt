
cmake_minimum_required(VERSION 2.8)

project(VoxelGame)

# Add deps subdirectory
add_subdirectory(dependencies)

# Get Include Directories
set (INCLUDE_DIRS include

	dependencies/glfw/include/GLFW
	dependencies/glew-cmake/include/GL
	
	dependencies/glm
	dependencies/glm/gtc
	dependencies/glm/gtx
	
	dependencies/Simple-OpenGL-Image-Library/src
	# dependencies/tinyobjloader
	# dependencies/tinyobjloader
	# dependencies/freealut-master/include/AL
	# dependencies/assimp/include/assimp
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
aux_source_directory(src/model SRC_LIST9)
aux_source_directory(src/model/world/chunk SRC_LIST10)
aux_source_directory(src/model/world/terrainGeneration SRC_LIST11)
aux_source_directory(src/model/world/entity SRC_LIST13)
aux_source_directory(src/gui SRC_LIST14)
aux_source_directory(src/gui/widget SRC_LIST15)

# Add executable to project
add_executable(${PROJECT_NAME}
${SRC_LIST} ${SRC_LIST2} ${SRC_LIST3} ${SRC_LIST4}
${SRC_LIST5} ${SRC_LIST6} ${SRC_LIST7} ${SRC_LIST8}
${SRC_LIST9} ${SRC_LIST10} ${SRC_LIST11} #${SRC_LIST12}
${SRC_LIST13} ${SRC_LIST14} ${SRC_LIST15}
)

# Link # assimp # alut # tinyobjloader
target_link_libraries(${PROJECT_NAME} libglew_static glfw SOIL)

add_definitions(-std=c++0x -Wextra)


