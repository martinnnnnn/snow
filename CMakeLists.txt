cmake_minimum_required(VERSION 3.16.3)


#### GENERAL CMAKE SETTINGS ####
project(snow)
message(STATUS "project name : ${PROJECT_NAME}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /MP /GR-")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_SUPPRESS_REGENERATION true)

#### SETTING PROJECT MAIN PATHS AND NAMES ####
set(SNOW_PROJECT snow)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${SNOW_PROJECT})

set(SNOW_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/src/")

set(SNOW_EXE_PATH "${CMAKE_SOURCE_DIR}/bin/${SNOW_PROJECT}")

#### GROUP CREATION ####
macro(GroupSources root_dir curdir)
    file(GLOB children RELATIVE ${root_dir}/${curdir}
            ${root_dir}/${curdir}/*)

    foreach(child ${children})
        if(IS_DIRECTORY ${root_dir}/${curdir}/${child})
            GroupSources(${root_dir} ${curdir}/${child})
        else()
            string(REPLACE "/" "\\" groupname ${curdir})
            source_group(${groupname} FILES ${curdir}/${child})

        endif()
    endforeach()
endmacro()


#### SETTING WINDOWS SDK PATHS ####
file(TO_CMAKE_PATH "$ENV{PROGRAMFILES\(X86\)}" _progFilePath)
set(WINDOWS_SDK_INCLUDES "${_progFilePath}/Windows Kits/10/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um/")
set(WINDOWS_SDK_LIBS "${_progFilePath}/Windows Kits/10/Lib/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um/x64")

#### ADDING SOURCE ####
add_subdirectory(src)



