cmake_minimum_required(VERSION 3.16)

# Название проекта
project(lad4desk VERSION 1.0 LANGUAGES CXX)

# Установить стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Найти Qt6 (если используете Qt5, замените на find_package(Qt5 COMPONENTS Widgets REQUIRED))
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Включить автоматическую обработку MOC, UIC, RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Список заголовочных файлов (Header Files)
set(HEADER_FILES
    addworkdialog.h
    bonusstrategy.h
    editworkdialog.h
    exceptions.h
    mainwindow.h
    payrolldepartment.h
    validators.h
    worktype.h
)

# Список исходных файлов (Source Files)
set(SOURCE_FILES
    addworkdialog.cpp
    bonusstrategy.cpp
    editworkdialog.cpp
    main.cpp
    mainwindow.cpp
    payrolldepartment.cpp
    validators.cpp
    worktype.cpp
)

# Список файлов форм (.ui files)
set(UI_FILES
    addworkdialog.ui
    editworkdialog.ui
    mainwindow.ui
)

# Добавить исполняемый файл
add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${UI_FILES}
)

# Подключить Qt6 Widgets
target_link_libraries(${PROJECT_NAME}
    Qt6::Widgets
)

# Включить каталоги для заголовочных файлов
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Установить свойства для Windows
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )

    # Опционально: установить иконку приложения
    # if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/icon.ico")
    #     set_target_properties(${PROJECT_NAME} PROPERTIES
    #         RC_ICONS "${CMAKE_CURRENT_SOURCE_DIR}/icon.ico"
    #     )
    # endif()
endif()

# Установить свойства для macOS
if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourcompany.lad4desk"
        MACOSX_BUNDLE_BUNDLE_NAME "lad4desk"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    )
endif()

# Для отладки: вывести список файлов
message(STATUS "Проект: ${PROJECT_NAME}")
message(STATUS "Исходные файлы: ${SOURCE_FILES}")
message(STATUS "Заголовочные файлы: ${HEADER_FILES}")
message(STATUS "Файлы UI: ${UI_FILES}")
