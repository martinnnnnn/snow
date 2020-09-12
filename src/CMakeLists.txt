


link_directories(
    ${WINDOWS_SDK_LIBS}
    ${CMAKE_SOURCE_DIR}/external/libs)

    file(GLOB_RECURSE SNOW_SOURCE_FILES "*.h" "*.hpp" "*.cpp" "*.inl")

    add_executable(${SNOW_PROJECT} ${SNOW_SOURCE_FILES} ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/external/headers/fmt/src/format.cc ${CMAKE_SOURCE_DIR}/external/headers/fmt/src/os.cc)
    
    set(LIBS
        SDL2
        SDL2main
        SDL2_image
        SDL2_ttf
        SDL2_mixer
        )
    
    target_include_directories(${SNOW_PROJECT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
    
    target_include_directories(${SNOW_PROJECT} PUBLIC
        ${SNOW_INCLUDE_PATH}
        ${CMAKE_SOURCE_DIR}/external/headers/SDL2-2.0.12/
        ${CMAKE_SOURCE_DIR}/external/headers/SDL2_image-2.0.5/
        ${CMAKE_SOURCE_DIR}/external/headers/SDL2_ttf-2.0.15/
        ${CMAKE_SOURCE_DIR}/external/headers/SDL2_mixer-2.0.4/
        ${CMAKE_SOURCE_DIR}/external/headers/fmt/
        )
    
    set_target_properties(${SNOW_PROJECT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SNOW_EXE_PATH})
    
    GroupSources(${CMAKE_CURRENT_SOURCE_DIR} snow)
    
    add_custom_command(TARGET ${SNOW_PROJECT} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/exe_root/$<CONFIGURATION>
            ${SNOW_EXE_PATH}/$<CONFIGURATION>)
    