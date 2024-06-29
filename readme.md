# Графическое приложение для карточных игр
# Выполнили студентки группы 8В32: Гекк С.С., Романчук С.Д., Тимохина В.А.
## Требования для запуска приложения
### Необходимые загрузки.
1. Операционная система:
   * __Windows__:  Windows 10 или выше.
   * __Linux__:  Любая дистрибуция Linux с поддержкой GCC (Ubuntu, Fedora, Debian, Arch Linux, и т.д.)
2. Среда разработки:
   * Visual Studio Code (VS Code):  [https://code.visualstudio.com/](https://code.visualstudio.com/)
3. Компилятор:
   * GCC (g++):  [https://gcc.gnu.org/](https://gcc.gnu.org/)
     * На __Windows__ используйте MinGW-w64: [https://www.mingw-w64.org/](https://www.mingw-w64.org/)
     * На __Linux__ GCC обычно уже установлен. 
4. Библиотека SFML:
   * [https://www.sfml-dev.org/](https://www.sfml-dev.org/)  - Скачайте архив с исходным кодом SFML.
### Настройка среды разработки.
1. VS Code:
   1. Установка расширений: 
       * C/C++:  [https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
       * CMake Tools: [https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
   2. Настройка CMake Tools:
       * Откройте папку с вашим проектом в VS Code.
       * Откройте палитру команд (Ctrl+Shift+P) и введите "CMake: Configure" для настройки CMake.
       * Выберите подходящий генератор (например, "MinGW Makefiles" для Windows).
       * Выберите папку для сборки (например, "build").
2. Linux:
   * Установка CMake:
        sudo apt-get update
        sudo apt-get install cmake
        (Для других дистрибутивов Linux могут быть другие команды установки)
### Последовательность сборки приложения:
1. Создание папки проекта:
   * Создайте новую папку для вашего проекта, например, "my_card_game".
2. Распаковка SFML:
   * Распакуйте скачанный архив SFML в папку с проектом.
3. Создание CMakeLists.txt:
   * Создайте файл CMakeLists.txt в корневой папке проекта с следующим содержимым:
     cmake_minimum_required(VERSION 3.10)
     project(my_card_game)
     * Настройка SFML
     set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SFML)
     include_directories(${SFML_DIR}/include)
     add_subdirectory(${SFML_DIR})
     * Файлы вашего проекта
     add_executable(my_card_game main.cpp) 
     target_link_libraries(my_card_game SFML_graphics SFML_window SFML_audio SFML_system)
     * Замените my_card_game на имя вашего проекта.
     * main.cpp -  главный файл вашего проекта.
     *  Убедитесь, что путь к папке SFML (SFML_DIR) верный.
4. Создание папки сборки:
   * Создайте папку "build" в корневой папке проекта.
5. Настройка CMake:
   * VS Code: Откройте палитру команд (Ctrl+Shift+P) и выберите "CMake: Configure".
   * __Linux__: Перейдите в папку "build" в терминале и запустите: cmake ..
6. Сборка проекта:
   * VS Code:  Откройте палитру команд и выберите "CMake: Build".
   * __Linux__:  Запустите команду: make
7. Запуск приложения:
   * VS Code:  В палитре команд выберите "CMake: Run".
   * __Linux__:  Перейдите в папку "build" в терминале и запустите: ./my_card_game