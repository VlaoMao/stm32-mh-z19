cmake_minimum_required(VERSION 3.5.0)
SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(stm32-mh-z19)
include (arm.toolchain)
SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "") #remove -rdynamic
enable_language(ASM)
SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
add_definitions(-DSTM32F1)

set(LIBOPENCM3_ROOT ${CMAKE_SOURCE_DIR}/lib/libopencm3)

set(PCD8544_ROOT ${CMAKE_SOURCE_DIR}/src/pcd8544)

set(MHZ19_ROOT ${CMAKE_SOURCE_DIR}/src/mh-z19)

#file(GLOB_RECURSE USER_SOURCES src/*.c)

include_directories(
        ${LIBOPENCM3_ROOT}/include
        ${PCD8544_ROOT}
        ${MHZ19_ROOT}
        inc)

link_directories(
        ${LIBOPENCM3_ROOT}/lib)

set(STM32F1_FLAGS "-mcpu=cortex-m3 -mthumb -mthumb-interwork -msoft-float")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall ${STM32F1_FLAGS} -Os -ffunction-sections -fdata-sections -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c++14 ${STM32F1_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_SOURCE_DIR}/libopencm3.ld -lopencm3_stm32f1 -nostartfiles -specs=nosys.specs -specs=nano.specs -Wl,--gc-sections")
# --specs=rdimon.specs


add_custom_target(libopencm3 make WORKING_DIRECTORY ${LIBOPENCM3_ROOT})

function(add_bin_from_elf bin elf)
    add_custom_target(${bin}
            ALL ${CMAKE_OBJCOPY}
            -Obinary ${elf} ${bin} DEPENDS ${elf})
endfunction(add_bin_from_elf)

add_subdirectory(src)
