cmake_minimum_required(VERSION 3.0.0)
project(packages VERSION 0.1.0)

# Set the output directory for executables outside of the build folder
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE FALSE)
# find_package(SDL2 REQUIRED)
# include_directories(${SDL2_INCLUDE_DIRS})
# include_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

# Add the -mconsole flag to ensure it's treated as a console application
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -municode")

add_executable(testberkleysocket main.cpp)

# Path to the directory containing SDL2 headers
set(SDL2_INCLUDE_DIR "C:/KUNSKAP/CODING_TRAINING/C++/berkley_sockets/lib/SDL2-devel-2.30.3-mingw/SDL2-2.30.3/x86_64-w64-mingw32/include")

# Path to the directory containing SDL2 libraries
set(SDL2_LIBRARY_DIR "C:/KUNSKAP/CODING_TRAINING/C++/berkley_sockets/lib/SDL2-devel-2.30.3-mingw/SDL2-2.30.3/x86_64-w64-mingw32/lib")

# Include SDL2 headers
target_include_directories(testberkleysocket PRIVATE ${SDL2_INCLUDE_DIR})

# Link against SDL2 library
target_link_libraries(testberkleysocket PRIVATE ${SDL2_LIBRARY_DIR}/libSDL2.dll.a)

# Set linker flags
target_link_options(testberkleysocket PRIVATE "-municode")

# Link against the Winsock library
# target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)

# target_include_directories(testberkleysocket PRIVATE C:/KUNSKAP/CODING_TRAINING/C++/berkley_sockets/lib/SDL2/x86_64-w64-mingw32/include/SDL2)
# target_link_libraries(testberkleysocket PRIVATE C:/KUNSKAP/CODING_TRAINING/C++/berkley_sockets/lib/SDL2/x86_64-w64-mingw32/lib/libSDL2.dll.a)

# Find all DLL files in the "lib" directory
file(GLOB DLL_FILES ${CMAKE_SOURCE_DIR}/lib/*.dll)

# # Copy DLL files to the binary output directory after build
# add_custom_command(TARGET testberkleysocket POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:testberkleysocket>"
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL_FILES} "$<TARGET_FILE_DIR:testberkleysocket>"
# )
