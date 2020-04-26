cmake_minimum_required(VERSION 3.15)

#Cross compiling toolchain:
set(CMAKE_TOOLCHAIN_FILE "toolchain.armclang" CACHE PATH "")

#top project:
project(appStartDemo VERSION 1.0
			LANGUAGES C CXX ASM)

#Options:
include(CMakeDependentOption)
option(USE_ARM_LIB_STARTUP "Use ARM Library's __main for scatter loading"  ON)

#Generate configuration header file:
configure_file(config.h.in  config.h)

#the top app target:
add_executable(appStartDemo 
						asm/basehw.S
						asm/reset.S
						asm/vectors.S
						my__main.c
						uart.c
						appmain.c
				)

set_source_files_properties(asm/basehw.S  PROPERTIES COMPILE_OPTIONS --target=aarch64-arm-none-eabi)
set_source_files_properties(asm/reset.S   PROPERTIES COMPILE_OPTIONS --target=aarch64-arm-none-eabi)
set_source_files_properties(asm/vectors.S PROPERTIES COMPILE_OPTIONS --target=aarch64-arm-none-eabi)

target_include_directories(appStartDemo PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_compile_options(appStartDemo PRIVATE -g -O0)
target_link_options(appStartDemo PRIVATE --scatter=${CMAKE_CURRENT_SOURCE_DIR}/v8A.scat --entry reset_handler --map)
set_target_properties(appStartDemo PROPERTIES SUFFIX .axf)

