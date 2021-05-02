cmake_minimum_required(VERSION 3.4.1)

project(foo)
add_compile_options(-Wall -O3)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL REQUIRED
    sdl2
    )

add_executable(${PROJECT_NAME}

    src/gplayer.cpp
    src/gplayer.hpp
    src/gsong.cpp
    src/gsong.hpp
    src/main.cpp

    src/resid/envelope.cc
    src/resid/extfilt.cc
    src/resid/filter.cc
    src/resid/pot.cc
    src/resid/sid.cc
    src/resid/version.cc
    src/resid/voice.cc
    src/resid/wave6581_PS_.cc
    src/resid/wave6581_PST.cc
    src/resid/wave6581_P_T.cc
    src/resid/wave6581__ST.cc
    src/resid/wave8580_PS_.cc
    src/resid/wave8580_PST.cc
    src/resid/wave8580_P_T.cc
    src/resid/wave8580__ST.cc
    src/resid/wave.cc

    )

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SDL_INCLUDE_DIRS}
    )

target_link_libraries(${PROJECT_NAME}
    ${SDL_LIBRARIES}
    )
