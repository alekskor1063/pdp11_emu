cmake_minimum_required(VERSION 3.15)
project(pdp11_emu C)

set(CMAKE_C_STANDARD 99)

add_executable(pdp11_emu main.c pdp.h run.c cmd.c display.c)