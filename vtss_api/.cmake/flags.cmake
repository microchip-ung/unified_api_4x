# Copyright (c) 2004-2018 Microsemi Corporation "Microsemi".
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

string (REPLACE " -" ";-" CXX_FLAGS        "${CMAKE_CXX_FLAGS}")
string (REPLACE " -" ";-" C_FLAGS          "${CMAKE_C_FLAGS}")
string (REPLACE " -" ";-" EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    LIST(APPEND CXX_FLAGS "-stdlib=libc++")
endif()

LIST(APPEND C_FLAGS   "-Wall -std=c99 -D_POSIX_C_SOURCE=199309L -D_BSD_SOURCE -D_DEFAULT_SOURCE")

list(REMOVE_DUPLICATES CXX_FLAGS)
list(REMOVE_DUPLICATES C_FLAGS)
list(REMOVE_DUPLICATES EXE_LINKER_FLAGS)

string (REPLACE ";-" " -" CXX_FLAGS        "${CXX_FLAGS}")
string (REPLACE ";-" " -" C_FLAGS          "${C_FLAGS}")
string (REPLACE ";-" " -" EXE_LINKER_FLAGS "${EXE_LINKER_FLAGS}")

SET(CMAKE_CXX_FLAGS        "${CXX_FLAGS}")
SET(CMAKE_C_FLAGS          "${C_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${EXE_LINKER_FLAGS}")

message(STATUS "Project name = ${PROJECT_NAME}")
message(STATUS "  Type       = ${CMAKE_BUILD_TYPE}")
message(STATUS "  cxx_flags  = ${CMAKE_CXX_FLAGS}")
message(STATUS "  c_flags    = ${CMAKE_C_FLAGS}")
