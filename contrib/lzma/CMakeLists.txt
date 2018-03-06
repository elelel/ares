cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(lzma LANGUAGES C)

set(SOURCES
  ${CONTRIB}/lzma/LzmaDec.c
  ${CONTRIB}/lzma/LzmaEnc.c
  ${CONTRIB}/lzma/LzFind.c)

add_library(lzma STATIC ${SOURCES})
target_compile_definitions(lzma PRIVATE "_7ZIP_ST")
