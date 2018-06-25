set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_minimum_required(VERSION 3.9)
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib ${PROJECT_SOURCE_DIR}/bin)
add_executable(
        RocksmithRPC
        Main.cpp
        RocksmithRPC.cpp
)
target_link_libraries(RocksmithRPC discord-rpc)

install(
        TARGETS RocksmithRPC
        RUNTIME
        DESTINATION "bin"
        CONFIGURATIONS Release
        BUNDLE
        DESTINATION "bin"
        CONFIGURATIONS Release
)