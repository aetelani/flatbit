cmake_minimum_required (VERSION 2.6)
project (flatbit)
add_executable(flatbit flatbit.c datacontainer.c storage.c storagepolicy.c filestorage.c memstorage.c)
include_directories (${flatbit_SOURCE_DIR}) 

SET (CMAKE_C_COMPILER             "/usr/bin/clang")
SET (CMAKE_C_FLAGS                "-Wall -std=c99")
SET (CMAKE_C_FLAGS_DEBUG          "-g")
SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")


configure_file (
  "${PROJECT_SOURCE_DIR}/flatbitconfig.h.in"
  "${PROJECT_BINARY_DIR}/flatbitconfig.h"
  )

include_directories("${PROJECT_BINARY_DIR}")
