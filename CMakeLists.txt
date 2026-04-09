cmake_minimum_required(VERSION 3.10)
project(memorc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build types
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Library target
add_library(memorc STATIC
    src/worker.cpp
    src/memorc.cpp
    src/worker_pool.cpp
    src/task_queue.cpp
    src/copy_task.cpp
)

# Include directories
target_include_directories(memorc PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/memorc>
    $<INSTALL_INTERFACE:include>
)

# Compiler flags
target_compile_options(memorc PRIVATE
    $<$<CONFIG:Debug>:-Wall -Wextra -Werror -g>
    $<$<CONFIG:Release>:-O3 -DNDEBUG>
)

# Link libraries
target_link_libraries(memorc PRIVATE 
    Threads::Threads
)

# Find required packages
find_package(Threads REQUIRED)

# Enable testing
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Install rules
install(TARGETS memorc
    EXPORT memorcTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

# Export targets
install(EXPORT memorcTargets
    FILE memorcConfig.cmake
    NAMESPACE memorc::
    DESTINATION lib/cmake/memorc
)

# Create config file
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/memorcConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/memorcConfig.cmake
    INSTALL_DESTINATION lib/cmake/memorc
)

write_basic_package_version_file(
    memorcConfigVersion.cmake
    VERSION 1.0.0
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/memorcConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/memorcConfigVersion.cmake
    DESTINATION lib/cmake/memorc
)

# Export all targets for use from the build tree
export(EXPORT memorcTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/memorcTargets.cmake"
    NAMESPACE memorc::
)
