option(USE_SYSTEM_LIBUV "Use system libuv" OFF)

if(NOT USE_SYSTEM_LIBUV)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libuv EXCLUDE_FROM_ALL)
    set_target_properties(uv_a PROPERTIES EXCLUDE_FROM_ALL TRUE POSITION_INDEPENDENT_CODE 1)
    add_library(Qv2ray::libuv ALIAS uv_a)
else()
    if(NOT WIN32)
        find_package(LibUV REQUIRED)
        add_library(Qv2ray::libuv ALIAS LibUV::LibUV)
    else()
        find_package(unofficial-libuv CONFIG REQUIRED)
        add_library(Qv2ray::libuv ALIAS unofficial::libuv::libuv)
    endif()
endif()

set(UVW_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/async.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/check.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/dns.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/emitter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/fs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/fs_event.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/fs_poll.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/idle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/lib.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/loop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/pipe.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/poll.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/prepare.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/process.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/signal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/stream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/tcp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/thread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/tty.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/util.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/work.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src/uvw/udp.cpp
    )

add_library(uvw STATIC ${UVW_SOURCES})
target_compile_definitions(uvw PRIVATE UVW_AS_LIB)
target_link_libraries(uvw Qv2ray::libuv)
set_target_properties(uvw PROPERTIES EXCLUDE_FROM_ALL TRUE)

target_include_directories(uvw PUBLIC
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uvw/src>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDE_DIR}/uvw/>")

add_library(Qv2ray::libuvw ALIAS uvw)

# BEGIN - the hack to install libuv and libuvw as static libraries
if(NOT BUILD_SHARED_LIBS)
    install(TARGETS uv_a EXPORT libuvTargets RUNTIME DESTINATION "lib/" ARCHIVE DESTINATION "lib/")
    install(TARGETS uvw  EXPORT uvwTargets   RUNTIME DESTINATION "lib/" ARCHIVE DESTINATION "lib/")

    install(EXPORT libuvTargets FILE libuvConfig.cmake NAMESPACE LibUV:: DESTINATION "lib/cmake/LibUV")
    install(EXPORT uvwTargets   FILE uvwConfig.cmake   NAMESPACE UVW::   DESTINATION "lib/cmake/uvw")

    export(TARGETS uv_a FILE _dummy-uv-a.cmake)
    export(TARGETS uvw  FILE _dummy-uvw.cmake)
endif()
# END  - the hack to install libuv and libuvw as static libraries
