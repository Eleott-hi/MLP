cmake_minimum_required(VERSION 3.5)
project(shared VERSION 0.1 LANGUAGES CXX)

# add_compile_options(-O3)
file(GLOB_RECURSE SOURCE_FILES *.cc *.h)

set(QT_LIBS PrintSupport)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_LIBS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_LIBS})

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

foreach(i ${QT_LIBS})
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::${i})
endforeach(i)
