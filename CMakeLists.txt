cmake_minimum_required (VERSION 3.1)
project(waveFunc)

set(EXE_NAME "WaveFunc")
set (CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)

find_package(GLEW)
if(NOT ${GLEW_FOUND})
	message(STATUS "GLEW not found.")
	set(GLEW_ROOT "glew" CACHE FILEPATH "Path to GLEW")
	get_filename_component(GLEW_ROOT ${GLEW_ROOT} ABSOLUTE)
	set(GLEW_INCLUDE_DIRS "${GLEW_ROOT}/include")
	file(GLOB GLEW_LIBRARIES "${GLEW_ROOT}/lib/*")
endif(NOT ${GLEW_FOUND})

set(GLM_ROOT "glm" CACHE FILEPATH "Path to GLM")
get_filename_component(GLM_ROOT ${GLM_ROOT} ABSOLUTE)
set(GLM_INCLUDE_DIRS "${GLM_ROOT}/include")

find_package(PkgConfig)
if(${PKG_CONFIG_FOUND})
	pkg_search_module(GLFW glfw3)
endif(${PKG_CONFIG_FOUND})

if(NOT ${GLFW_FOUND})
	message(STATUS "GLFW not found.")
	set(GLFW_ROOT "glfw" CACHE FILEPATH "Path to GLFW")
	get_filename_component(GLFW_ROOT ${GLFW_ROOT} ABSOLUTE)
	set(GLFW_INCLUDE_DIRS "${GLFW_ROOT}/include")
	file(GLOB GLFW_LIBRARIES "${GLFW_ROOT}/lib/*")
endif(NOT ${GLFW_FOUND})

file(GLOB SHADERS "${CMAKE_SOURCE_DIR}/shaders/*")

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
set(INSTALL_DIR "bin" CACHE FILEPATH "Installation path.")

add_subdirectory(src)
