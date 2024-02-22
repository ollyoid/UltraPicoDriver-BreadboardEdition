# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(ultra-pico-driver C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(ultra-pico-driver ultra-pico-driver.c )

pico_generate_pio_header(ultra-pico-driver ${CMAKE_CURRENT_LIST_DIR}/phase.pio)

pico_set_program_name(ultra-pico-driver "ultra-pico-driver")
pico_set_program_version(ultra-pico-driver "0.1")

pico_enable_stdio_uart(ultra-pico-driver 1)
pico_enable_stdio_usb(ultra-pico-driver 1)

# Add the standard library to the build
target_link_libraries(ultra-pico-driver
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(ultra-pico-driver PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(ultra-pico-driver
        hardware_dma
        hardware_pio
        hardware_clocks
        hardware_pwm
        )

pico_add_extra_outputs(ultra-pico-driver)

