file(GLOB SOURCES "*.cpp" "*.c")

add_library(elfStruct ${SOURCES})

target_include_directories(elfStruct PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
)