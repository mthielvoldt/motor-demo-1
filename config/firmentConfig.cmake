
# The location of the firment directory
set(FIRMENT_DIR "${PROJECT_SOURCE_DIR}/firment")
set(APP_FW_CONFIG_DIR "${PROJECT_SOURCE_DIR}/Src/fmt/config")
set(PORT_DIR "${FIRMENT_DIR}/firmware/port/stm32")
set(CMSIS_FAMILY_DIR "${PROJECT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32G4xx")

# These can be provided by firment, but can also be project specified (as here)
set(SYSTEM_FILE "${PROJECT_SOURCE_DIR}/Src/system_stm32g4xx.c")
set(HAL_DIR "${PROJECT_SOURCE_DIR}/Drivers/STM32G4xx_HAL_Driver")

# PCB should be set on cmake invocation via cmake ... -DPCB=<num>. 
set(PCB_DIR ${APP_FW_CONFIG_DIR}/pcb${PCB})

## Optional modules
set(ENABLE_WAVEFORM 0)
set(ENABLE_GHOST_PROBE 1)
include(${FIRMENT_DIR}/cmake-tools/fmtTransport.cmake)

# update_page_size is used in:
# - web-ui for <Image> upload widget (via /web-ui/src/generated/flashPage.ts)
# - fmt_update.c from configured fmt_update.h
set(UPDATE_PAGE_SIZE 256)
set(UPDATE_SUPPORTED 0)
set(DATA_MSG_PAYLOAD_SIZE_MAX 32)
set(LOG_TEXT_MAX_SIZE      50)
set(PROBE_SIGNAL_MAX_COUNT 6) # Todo: remove


message(STATUS "Update page size: ${UPDATE_PAGE_SIZE}")
message(STATUS "Message payload size max: ${DATA_MSG_PAYLOAD_SIZE_MAX}")
message(STATUS "Log text max size: ${LOG_TEXT_MAX_SIZE}")
