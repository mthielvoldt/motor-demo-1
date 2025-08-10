cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
message("Build type: " ${CMAKE_BUILD_TYPE})

set(PROJECT_CONFIG_DIR ${CMAKE_CURRENT_LIST_DIR}/config)
include(${PROJECT_CONFIG_DIR}/version.cmake)

set(CMAKE_PROJECT_NAME MCSDK-project1)
project(${CMAKE_PROJECT_NAME} VERSION ${VERSION_SEM} LANGUAGES C ASM)

### begin Firment
include(${PROJECT_CONFIG_DIR}/firmentConfig.cmake)     # no deps
include(${PROJECT_CONFIG_DIR}/mcuCompileOptions.cmake) # after firmentConfig

add_subdirectory(${PORT_DIR} ${MCU_FAMILY}) # provides MCUPort library.
add_subdirectory(${FIRMENT_DIR} firment)
### end Firment

add_executable(${CMAKE_PROJECT_NAME})

# Using link order to have firment libs override certain function definitions. 
# Specifically in this application: USART2_IRQHandler(), HAL_UART_MspInit()
target_link_libraries(${CMAKE_PROJECT_NAME}
  stm32cubemx
  FirmentFW
  MCUPort
)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)


target_sources(${CMAKE_PROJECT_NAME} PRIVATE
  Src/fmt/project_comms.c
)


# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)


