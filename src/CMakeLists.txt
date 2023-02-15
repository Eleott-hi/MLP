cmake_minimum_required(VERSION 3.5)

project(MLP VERSION 0.1 LANGUAGES CXX)

add_compile_options(-O3)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE

    # RELEASE
    DEBUG
)

set(QT_LIBS Widgets)
set(LIBS
    shared

    # common
)
set(EXCLUDE_DIRS tests ${LIBS})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_LIBS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_LIBS})

include_directories(model view ${LIBS})

foreach(i ${LIBS})
    add_subdirectory(${i})
endforeach(i)

file(GLOB_RECURSE PROJECT_SOURCES *.cc *.h *.ui)

foreach(i ${EXCLUDE_DIRS})
    message("Excluding dir: " ${i})
    list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*${i}.*")
endforeach()

foreach(i ${PROJECT_SOURCES})
    message("Project file: " ${i})
endforeach()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION ${PROJECT_SOURCES})
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
    else()
        add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
    endif()
endif()

foreach(i ${QT_LIBS})
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::${i})
endforeach(i)

foreach(i ${LIBS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${i})
endforeach(i)

if(CMAKE_BUILD_TYPE STREQUAL RELEASE)
    # target_compile_definitions(shared PUBLIC
    # QT_NO_DEBUG_OUTPUT
    # QT_NO_INFO_OUTPUT
    # QT_NO_WARNING_OUTPUT
    # )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
