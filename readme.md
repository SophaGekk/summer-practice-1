# Графическое приложение для карточных игр
# Выполнили студенты группы 8В32: Гекк С.С., Романчук С.Д., Тимохина В.А.
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
   * [https://www.sfml-dev.org/](https://www.sfml-dev.org/)  - библиотека SFML, подключенная через CMakeLists 
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
1. Скачивание проекта:
   * Скачайте проект с GitHub через команду git clone <адрес_репозитория>. 
2. Создание папки сборки:
   * Создайте папку "build" в корневой папке проекта.
3. **Перемещение ресурсов:**
   * Перенесите файлы из папки `resources/необходимо перенести в build/` в папку `build/`.
     * Список файлов: `openal32.dll`, `sfml-audio-2.dll`, `sfml-audio-d-2.dll`, `sfml-graphics-2.dll`, `sfml-graphics-d-2.dll`, `sfml-network-2.dll`, `sfml-network-d-2.dll`, `sfml-system-2.dll`, `sfml-system-d-2.dll`, `sfml-window-2.dll`, `sfml-window-d-2.dll`.
4. **Настройка CMake:**
   * **VS Code:** Откройте палитру команд (Ctrl+Shift+P) и выберите "CMake: Configure".
   * **Linux:** Перейдите в папку "build" в терминале и запустите: `cmake ..`
5. **Сборка проекта:**
   * **VS Code:**  Откройте палитру команд и выберите "CMake: Build".
   * **Linux:**  Запустите команду: `make`
6. **Запуск приложения:**
   * **VS Code:**  В палитре команд выберите "CMake: Debug".
   * **Linux:**  Перейдите в папку "build" в терминале и запустите: `./my_card_game`

**Объяснение:**

* **Перемещение ресурсов:**  Это необходимо для того, чтобы библиотеки SFML были доступны во время выполнения приложения.  При сборке проекта CMake создает ссылки на эти библиотеки, но сами файлы библиотек должны быть в той же папке, что и исполняемый файл.

**Важно:**
* **Не создавайте никакие папки в папке `build/`**, просто скопируйте туда файлы.

**Примечание:**
* **Все многопользовательские игры реализованы на 4 игроков с разным соотношением бот-человек**
* **Подкидной дурак коректно работает на 4 игроков - людей, ботов не научили**
* **При загрузке сохранения в игре в "Червы" может возникнуть ошибка и у игроков далее будет разное количество карт, что может привести к ошибке, когда у всех должны были закончатся карты. ошибка может быть связана с тем, что нет возможности сохранить статус игры из-за конфликта типа данных**