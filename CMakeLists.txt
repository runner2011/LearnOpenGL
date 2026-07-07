cmake_minimum_required(VERSION 3.16)

project(LearnOpenGL LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(LearnOpenGL
    Source/glad.c
    Source/instancing_quads.cpp
)

target_include_directories(LearnOpenGL PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/includes"
)

function(learnopengl_prefer_target_includes dependency_target)
    get_target_property(include_dirs "${dependency_target}" INTERFACE_INCLUDE_DIRECTORIES)
    if(include_dirs)
        target_include_directories(LearnOpenGL BEFORE PRIVATE ${include_dirs})
    endif()
endfunction()

find_package(OpenGL REQUIRED)

find_package(glfw3 CONFIG QUIET)
if(TARGET glfw)
    set(LEARNOPENGL_GLFW_TARGET glfw)
elseif(TARGET glfw3)
    set(LEARNOPENGL_GLFW_TARGET glfw3)
else()
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(GLFW3 QUIET IMPORTED_TARGET glfw3)
    endif()

    if(TARGET PkgConfig::GLFW3)
        set(LEARNOPENGL_GLFW_TARGET PkgConfig::GLFW3)
    elseif(WIN32 AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib")
        add_library(learnopengl_glfw STATIC IMPORTED)
        set_target_properties(learnopengl_glfw PROPERTIES
            IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/includes"
        )
        set(LEARNOPENGL_GLFW_TARGET learnopengl_glfw)
    else()
        message(FATAL_ERROR "GLFW was not found. Install glfw3 development files, or provide a CMake package for glfw3.")
    endif()
endif()
learnopengl_prefer_target_includes(${LEARNOPENGL_GLFW_TARGET})

find_package(assimp CONFIG QUIET)
if(TARGET assimp::assimp)
    set(LEARNOPENGL_ASSIMP_TARGET assimp::assimp)
elseif(TARGET assimp)
    set(LEARNOPENGL_ASSIMP_TARGET assimp)
else()
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(ASSIMP QUIET IMPORTED_TARGET assimp)
    endif()

    if(TARGET PkgConfig::ASSIMP)
        set(LEARNOPENGL_ASSIMP_TARGET PkgConfig::ASSIMP)
    elseif(WIN32 AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp-vc142-mt.lib")
        add_library(learnopengl_assimp UNKNOWN IMPORTED)
        set_target_properties(learnopengl_assimp PROPERTIES
            IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp-vc142-mt.lib"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/includes"
        )
        set(LEARNOPENGL_ASSIMP_TARGET learnopengl_assimp)
    else()
        message(FATAL_ERROR "Assimp was not found. Install assimp development files, or provide a CMake package for assimp.")
    endif()
endif()
learnopengl_prefer_target_includes(${LEARNOPENGL_ASSIMP_TARGET})

target_link_libraries(LearnOpenGL PRIVATE
    OpenGL::GL
    ${LEARNOPENGL_GLFW_TARGET}
    ${LEARNOPENGL_ASSIMP_TARGET}
    ${CMAKE_DL_LIBS}
)

if(MSVC)
    target_compile_definitions(LearnOpenGL PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

add_custom_command(TARGET LearnOpenGL POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/Source"
            "$<TARGET_FILE_DIR:LearnOpenGL>/../Source"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/Res"
            "$<TARGET_FILE_DIR:LearnOpenGL>/../Res"
)
