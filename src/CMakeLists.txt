########################## QT ######################################
find_package(Qt5 COMPONENTS Widgets OpenGL Gui)

find_package(GDAL REQUIRED)

add_subdirectory(Vendors)
add_subdirectory(App)