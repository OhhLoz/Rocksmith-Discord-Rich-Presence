include_directories(${PROJECT_SOURCE_DIR}/include)
add_executable(
    RocksmithRPC
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