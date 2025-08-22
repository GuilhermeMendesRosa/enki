cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(VisualTest)

# Encontrar Qt5
find_package(Qt5 REQUIRED COMPONENTS Core Widgets OpenGL)

# Encontrar OpenGL
find_package(OpenGL REQUIRED)

# Incluir diretórios
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/enki)
include_directories(${CMAKE_SOURCE_DIR}/viewer)

# Exemplo Avançado
set(VISUAL_TEST_SOURCES
    VisualTest.cpp
)

add_executable(visual_test ${VISUAL_TEST_SOURCES})

set_target_properties(visual_test PROPERTIES
    AUTOMOC ON
)

target_link_libraries(visual_test
    enki
    enkiviewer
    Qt5::Core
    Qt5::Widgets
    Qt5::OpenGL
    ${OPENGL_LIBRARIES}
)

# Exemplo Básico
set(BASIC_EXAMPLE_SOURCES
    BasicExample.cpp
)

add_executable(basic_example ${BASIC_EXAMPLE_SOURCES})

set_target_properties(basic_example PROPERTIES
    AUTOMOC ON
)

target_link_libraries(basic_example
    enki
    enkiviewer
    Qt5::Core
    Qt5::Widgets
    Qt5::OpenGL
    ${OPENGL_LIBRARIES}
)

# Definir padrão C++
set_property(TARGET visual_test PROPERTY CXX_STANDARD 11)
set_property(TARGET visual_test PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET basic_example PROPERTY CXX_STANDARD 11)
set_property(TARGET basic_example PROPERTY CXX_STANDARD_REQUIRED ON)
