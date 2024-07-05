cmake_minimum_required(VERSION 3.21)
project(GameMenu VERSION 0.0.1 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(GameMenu main.cpp GameMenu.cpp GameMenu.h AssetManager.cpp AssetManager.h Animator.cpp Animator.h menupik.h menupik.cpp pikdam.h pikdam.cpp menudurak.h menudurak.cpp durak.h durak.cpp menuhearts.h menuhearts.cpp Hearts.cpp Hearts.h HeartsBOT.cpp HeartsBOT.h) 

# если ошибка зугрзуки ресурсов window.png и аудио раскоменть это 
# add_custom_command(TARGET GameMenu POST_BUILD
                   
#                    COMMAND ${CMAKE_COMMAND}
#                    ARGS -E copy_directory
#                         ${PROJECT_SOURCE_DIR}/resources
#                         $<TARGET_FILE_DIR:GameMenu>/resources
#                    VERBATIM)

target_link_libraries(GameMenu PRIVATE sfml-graphics sfml-system sfml-window sfml-audio )
# и закоменть это
# Добавление команды копирования ресурсов
add_custom_command(
    OUTPUT resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory resources ${CMAKE_BINARY_DIR}/resources
    DEPENDS resources
)
# до сюда !!!!!!!!

target_compile_features(GameMenu PRIVATE cxx_std_14)

# add_custom_command(
#   TARGET GameMenu POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy 
#     ${PROJECT_SOURCE_DIR}/openal32.dll
#     $<TARGET_FILE_DIR:GameMenu>/openal32.dll)

# if (WIN32 AND BUILD_SHARED_LIBS)
#     add_custom_command(TARGET GameMenu POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:GameMenu> $<TARGET_FILE_DIR:GameMenu> COMMAND_EXPAND_LISTS)
# endif()

# install(TARGETS GameMenu)

# Директория с СMakeLists.txt
# cmake -S . -B build
# cmake --build build --config Release
# cmake --build build --config Debug
# Для запуска проекта выбрать активный проект