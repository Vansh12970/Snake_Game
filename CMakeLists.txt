cmake_minimum_required(VERSION 3.16)
project(SnakeGame)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set SFML path (adjust this path to where you extracted SFML)
# Adjust the path below to match your compiler architecture (e.g., "x64" or "x86")
set(SFML_DIR "C:/SFML-2.6.1/lib/cmake/SFML")

# Find SFML components
find_package(SFML 2.6.1 REQUIRED COMPONENTS system window graphics audio)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} 
    sfml-system 
    sfml-window 
    sfml-graphics 
    sfml-audio
)

# Copy SFML DLLs to output directory on Windows
if(WIN32)
    # Get SFML DLL directory
    get_target_property(SFML_SYSTEM_LOCATION sfml-system IMPORTED_LOCATION_RELEASE)
    get_filename_component(SFML_DLL_DIR ${SFML_SYSTEM_LOCATION} DIRECTORY)
    
    # Copy DLLs after build
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_DLL_DIR}/sfml-system-2.dll"
        "${SFML_DLL_DIR}/sfml-window-2.dll" 
        "${SFML_DLL_DIR}/sfml-graphics-2.dll"
        "${SFML_DLL_DIR}/sfml-audio-2.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()

# Set working directory for Visual Studio
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")