cmake_minimum_required(VERSION 3.27.1)

project(LMCube
    VERSION 0.1.0
    LANGUAGES C CXX
)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
    message(FATAL_ERROR "In-source builds are not allowed")
endif()

# the value of CMAKE_CXX_STANDARD serves as a default for all subsequent targets
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# don't allow the usage of compiler-specific extensions to improve portability
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

# include(${CMAKE_SOURCE_DIR}/cmake/get_cpm.cmake)
add_subdirectory(src)
