

cmake_minimum_required (VERSION 3.8)
project ("opengl_tutorials")
set (CMAKE_CXX_STANDARD 20)




find_package(OpenGL REQUIRED)

include_directories(
	src
	include
	external
	external/glm
	external/glad/include
	external/stb
	resources
	tuto
)

set(EXT_SOURCES
	external/glad/src/glad.c
	external/stb/stb_image.c
)


add_executable(${PROJECT_NAME} 
	src/main
	src/shader.cpp 
	src/camera.cpp
	src/sphere.cpp
	${EXT_SOURCES}
)

target_link_directories(${PROJECT_NAME} PRIVATE 
	${CMAKE_SOURCE_DIR}/external/GLFW
	${CMAKE_SOURCE_DIR}/external/glm
)


target_link_libraries(${PROJECT_NAME}
    PRIVATE
    glfw3
    OpenGL::GL
	glm_static
)

add_custom_target(copy-runtime-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        ${CMAKE_BINARY_DIR}/resources
)
add_dependencies(${PROJECT_NAME} copy-runtime-files)