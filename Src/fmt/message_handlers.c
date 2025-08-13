// Interface
#include "config/message_handlers.h"

// Dependencies
#include <fmt_comms.h>
// #include <build_time.h> // Generate this?

bool initFirmentComms(void)
{
  bool success = fmt_initComms();
  return success;
}

void handleTelemetry(void)
{
  static uint32_t count = 0, current = 0;
  switch (count++)
  {
  case 0:
  {
    fmt_sendMsg((Top){
        .which_sub = Top_MotorTlm_tag,
        .sub = {
            .MotorTlm = {
                .currentMa = current++,
                .busVoltageV = 0,
            }}});
    break;
  }
  case 999:
  {
    count = 0;
    break;
  }
  }
}

void handleMotorCtl(MotorCtl msg)
{
}

#define USE_Reset
void handleReset(Reset msg)
{
}

