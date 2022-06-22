set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QML_IMPORT_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} CACHE PATH "Path to qml modules")


add_subdirectory(app)
add_subdirectory(qmlmodules)
