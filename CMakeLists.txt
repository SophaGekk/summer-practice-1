# cmake_minimum_required(VERSION 3.21)
# project(durak VERSION 0.0.1 LANGUAGES CXX)

# include(FetchContent)
# FetchContent_Declare(SFML
#     GIT_REPOSITORY https://github.com/SFML/SFML.git
#     GIT_TAG 2.6.x)
# FetchContent_MakeAvailable(SFML)

# add_executable(durak durak.cpp)
# target_link_libraries(durak PRIVATE sfml-graphics sfml-system sfml-window sfml-audio )
# target_compile_features(durak PRIVATE cxx_std_14)

cmake_minimum_required(VERSION 3.21)
project(durak VERSION 0.0.1 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)
add_executable(durak main.cpp durak.cpp)
target_link_libraries(durak PRIVATE sfml-graphics sfml-system sfml-window sfml-audio )

set(CMAKE_CXX_STANDARD 17)

# Укажите папку с ресурсами
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")

# Добавление команды копирования ресурсов
add_custom_command(
    TARGET durak POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCE_DIR} $<TARGET_FILE_DIR:durak>/resources
)

target_compile_features(durak PRIVATE cxx_std_14)