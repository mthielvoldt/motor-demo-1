## This file sets variables that are determined by MCU selection.

if(PCB STREQUAL "0")
  set(MCU_FAMILY "stm32g4")
  set(MCU_VARIANT "stm32g431")
  add_compile_definitions("STM32G431xx")
endif()

add_compile_definitions("${MCU_FAMILY}")

message(STATUS "PCB: ${PCB}")
message(STATUS "MCU: ${MCU_VARIANT}")
