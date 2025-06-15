cmake_minimum_required(VERSION 3.16)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)

# Set SFML paths
set(SFML_INCLUDE_DIR "C:/SFML-3.0.0/include")
set(SFML_LIBRARY_DIR "C:/SFML-3.0.0/lib")

# Include SFML headers
include_directories(${SFML_INCLUDE_DIR})
link_directories(${SFML_LIBRARY_DIR})

# Add your source files here
add_executable(SnakeGame 
    main.cpp
    Game.cpp
)

# Link SFML libraries directly
target_link_libraries(SnakeGame 
    sfml-graphics
    sfml-window
    sfml-audio
    sfml-network
    sfml-system
)

# Copy SFML DLLs to output directory (Windows only)
if(WIN32)
    add_custom_command(TARGET SnakeGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "C:/SFML-3.0.0/bin"
        $<TARGET_FILE_DIR:SnakeGame>)
endif()

# Set the output directory
set_target_properties(SnakeGame PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)