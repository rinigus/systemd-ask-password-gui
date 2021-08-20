project(systemd-ask-password-gui CXX)
cmake_minimum_required(VERSION 3.5)

find_package (Qt5 COMPONENTS Core Qml Gui Quick REQUIRED)

include(FindPkgConfig)
pkg_search_module(SAILFISH sailfishapp REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(REPLY_PASSWORD_CMD "/usr/lib/systemd/systemd-reply-password" CACHE STRING "Full path for systemd-reply-password")

# sources

## QML sources: installed in subdir
file(GLOB QML_SRC qml/*.qml)

## C++ sources
file(GLOB CXX_SRC src/*.cpp)
file(GLOB CXX_HEADERS src/*.h)

## Translations
FILE(GLOB TsFiles "translations/*.ts")

add_custom_target(Sources SOURCES
    ${QML_SRC}
    ${CXX_SRC} ${CXX_HEADERS}
    ${TsFiles}
    rpm/systemd-ask-password-gui.spec
    )

# compilation
add_compile_options(
    -DREPLY_PASSWORD_CMD="${REPLY_PASSWORD_CMD}"
)

add_executable(systemd-ask-password-gui ${CXX_SRC} ${CXX_HEADERS})

target_compile_definitions(systemd-ask-password-gui PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)
target_include_directories(systemd-ask-password-gui PRIVATE
    $<BUILD_INTERFACE:
    ${SAILFISH_INCLUDE_DIRS}
>)
target_link_libraries(systemd-ask-password-gui
    Qt5::Quick
    ${SAILFISH_LDFLAGS}
)

# installs
install(TARGETS systemd-ask-password-gui
    RUNTIME DESTINATION bin
)
install(DIRECTORY qml
    DESTINATION share/systemd-ask-password-gui
)
install(DIRECTORY translations
    DESTINATION share/systemd-ask-password-gui
    FILES_MATCHING PATTERN "*.qm"
)
