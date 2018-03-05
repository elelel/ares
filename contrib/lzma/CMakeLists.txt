cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(lzma LANGUAGES C)

set(SOURCES ${CONTRIB}/lzma/LzmaDec.c)

add_library(lzma STATIC ${SOURCES})
