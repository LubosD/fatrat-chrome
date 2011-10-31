project(fatrat-chrome)

cmake_minimum_required(VERSION 2.4.0)
if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

INCLUDE(FindPkgConfig)

set(CMAKE_MODULE_PATH cmake_modules)

ADD_DEFINITIONS(-DXP_UNIX)
if(CMAKE_BUILD_TYPE MATCHES Debug)
	ADD_DEFINITIONS(-ggdb)
	ADD_DEFINITIONS(-DDEBUG)
	ADD_DEFINITIONS(-Wall)
endif(CMAKE_BUILD_TYPE MATCHES Debug)

include_directories(./ ${CMAKE_CURRENT_BINARY_DIR})

pkg_check_modules(libxul REQUIRED "libxul >= 1.9.0")

if(libxul_VERSION GREATER 1.9.2 OR libxul_VERSION EQUAL 1.9.2)
	set(XUL_1_9_2 TRUE)
endif(libxul_VERSION GREATER 1.9.2 OR libxul_VERSION EQUAL 1.9.2)

include_directories(${libxul_INCLUDE_DIRS})

CONFIGURE_FILE(config.h.in config.h)

set(fatrat-chrome_SRCS
	src/np_entry.cpp
	src/npn_gate.cpp
	src/npp_gate.cpp
	src/plugin.cpp
	)


add_library(fatrat-chrome SHARED ${fatrat-chrome_SRCS})

target_link_libraries(fatrat-chrome)

message(STATUS "******** After the NPAPI plugin is compiled, run ./data/package.sh ${CMAKE_BINARY_DIR}/libfatrat-chrome.so to create a .crx file")
message(STATUS "******** You may have to terminate Chrome/Chromium before running that command")

# execute_process(COMMAND data/package.sh "${CMAKE_BINARY_DIR}/lib	fatrat-chrome.so")
