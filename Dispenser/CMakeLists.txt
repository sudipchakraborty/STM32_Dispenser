cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Enable C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME Dispenser)

# Enable compile command to ease indexing
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME} C CXX ASM)
message("Build type: " ${CMAKE_BUILD_TYPE})

# Create executable
add_executable(${CMAKE_PROJECT_NAME})
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES SUFFIX ".elf")


# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)
# Add custom modules
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    lib/Bios/Bios.cpp
    lib/Led/Led.cpp
    lib/Debug/Debug.cpp
    lib/Hx711/Hx711.cpp
)


target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    lib
)


# Remove wrong libob.a dependency
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Link libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
)
