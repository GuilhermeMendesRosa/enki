include_directories (${PROJECT_SOURCE_DIR})
link_directories (${PROJECT_BINARY_DIR})

# Procurar Qt5 para o viewer
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Network REQUIRED)

if(Qt5Core_FOUND AND Qt5Widgets_FOUND AND Qt5OpenGL_FOUND AND Qt5Xml_FOUND AND Qt5Network_FOUND)
    # Habilitar MOC automático
    set(CMAKE_AUTOMOC ON)
    
    add_executable(enkiSocketControl enkiSocketControl.cpp)
    target_link_libraries(enkiSocketControl enki enkiviewer)
    
    # Link Qt5 libraries
    target_link_libraries(enkiSocketControl Qt5::Core Qt5::Widgets Qt5::OpenGL Qt5::Xml Qt5::Network)
    
    message(STATUS "Socket Control example will be built")
else()
    message(STATUS "Socket Control example will NOT be built because Qt5 was not found")
endif()
