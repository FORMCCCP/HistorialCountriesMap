cmake_minimum_required(VERSION 3.16)

project(HistorialCountriesMaps VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_executable(appHistorialCountriesMaps
    main.cpp
)

qt_add_qml_module(appHistorialCountriesMaps
    URI HistorialCountriesMaps
    VERSION 1.0
    QML_FILES
        qml/Main.qml
    SOURCES
        src/control/totalcontroller.h
        src/control/totalcontroller.cpp
        src/view/mapview.h
        src/view/mapview.cpp
        src/domain/region.h
        src/domain/region.cpp
        src/rendering/renderer.h
        src/rendering/renderer.cpp
        src/rendering/nodebuilder.h
        src/rendering/nodebuilder.cpp
        src/domain/Tile.h
        src/projection/geoprojection.h
        src/projection/geoprojection.cpp
        src/data/tilestore.h
        src/data/tilestore.cpp
        src/rendering/tileloader.h
        src/rendering/tileloader.cpp
        src/tools/threadpool.h
        src/tools/threadpool.cpp
        src/tools/mainthreaddispatcher.h
        src/tools/mainthreaddispatcher.cpp
        src/projection/tilemath.h
        src/projection/tilemath.cpp
        src/data/sqlrepository.h
        src/data/sqlrepository.cpp
        src/projection/geotriangulate.h

        src/third_party/nlohmann/json.hpp
        src/third_party/sqlite3/sqlite3.c
        src/third_party/sqlite3/sqlite3.h
        src/third_party/tri/earcut.hpp
)

target_include_directories(appHistorialCountriesMaps PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/control
    ${CMAKE_CURRENT_SOURCE_DIR}/src/view
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data
    ${CMAKE_CURRENT_SOURCE_DIR}/src/domain
    ${CMAKE_CURRENT_SOURCE_DIR}/src/interaction
    ${CMAKE_CURRENT_SOURCE_DIR}/src/projection
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tools
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rendering
    ${CMAKE_CURRENT_SOURCE_DIR}/src/third_party
    ${CMAKE_CURRENT_SOURCE_DIR}/src/third_party/sqlite3
    ${CMAKE_CURRENT_SOURCE_DIR}/src/third_party/tri
)





# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appHistorialCountriesMaps PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appHistorialCountriesMaps
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)



target_link_libraries(appHistorialCountriesMaps
    PRIVATE Qt6::Quick
)

install(TARGETS appHistorialCountriesMaps
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
