cmake_minimum_required(VERSION 3.10.0)

project(P_VS_Z VERSION 0.1.0 LANGUAGES C CXX)
# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET ${PROJECT_NAME})

set(SOURCES 
    src/main.cpp
    src/game.cpp
    src/object.cpp
    src/scene.cpp
    src/plant.cpp
    src/scene_main.cpp
    src/scene_start.cpp
    src/sunshine.cpp
    src/zombie.cpp
    src/bullet.cpp
    src/actor.cpp
)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(${TARGET} ${SOURCES})
if(MSVC)
    add_compile_options(/utf-8)
endif()
# 修正包名（使用正确的大小写）
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(glm REQUIRED)

# 包含目录
target_include_directories(${TARGET} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

# 链接库 - 根据 vcpkg 的输出使用正确的目标名
target_link_libraries(${TARGET} PRIVATE
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
    glm::glm
)

# 复制资源文件夹 - 使用正确的目标名称 ${TARGET}
add_custom_command(TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/res
    $<TARGET_FILE_DIR:${TARGET}>/res
    COMMENT "Copying resource files..."
)

# 在 Windows 上复制 DLL
if(WIN32)
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>>"
        $<TARGET_FILE_DIR:${TARGET}>
    )
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:$<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>>"
        $<TARGET_FILE_DIR:${TARGET}>
    )
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:$<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>>"
        $<TARGET_FILE_DIR:${TARGET}>
    )
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:$<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>>"
        $<TARGET_FILE_DIR:${TARGET}>
    )
endif()

# "VCPKG_TARGET_TRIPLET": "x64-windows"
# "VCPKG_INSTALLED_DIR": "${sourceDir}/vcpkg_installed"