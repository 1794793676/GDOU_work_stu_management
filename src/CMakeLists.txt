set(SOURCES
    main.cpp
    mainwindow.cpp
    form.cpp
)

set(HEADERS
    mainwindow.h
    form.h
)

set(UIS
    mainwindow.ui
    form.ui
)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

qt5_wrap_ui(UI_HEADERS ${UIS})
qt5_wrap_cpp(MOC_SOURCES ${HEADERS})

add_executable(TEST8
    ${SOURCES}
    ${HEADERS}
    ${UIS}
    ${MOC_SOURCES}
    ${UI_HEADERS}
)

# 资源文件
set(RESOURCES ../resources/image.qrc)
qt5_add_resources(QRC_SOURCES ${RESOURCES})
target_sources(TEST8 PRIVATE ${QRC_SOURCES})

target_link_libraries(TEST8 Qt5::Widgets)

install(TARGETS TEST8
    RUNTIME DESTINATION bin
) 