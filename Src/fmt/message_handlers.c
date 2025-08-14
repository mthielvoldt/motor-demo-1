// Interface
#include "config/message_handlers.h"

#include <mc_interface.h>       // MC_StartMotor()
#include <mc_config.h>          // extern Mci[]
#include <bus_voltage_sensor.h> // VBS_GetAvBusVoltage_V()

// Dependencies
#include <fmt_comms.h>
#include <stdlib.h>
// #include <build_time.h> // Generate this?

MCI_Handle_t *motor = &Mci[0];
BusVoltageSensor_Handle_t *BusVoltageSensor = &BusVoltageSensor_M1._Super;

static bool motorRunning(MCI_Handle_t *motor);

bool initFirmentComms(void)
{
  bool ok = fmt_initComms();
  return ok;
}

void handleTelemetry(void)
{
  static uint32_t count = 0, uptime = 0;
  switch (count++)
  {
  case 0:
  {
    uint16_t busVoltage = VBS_GetAvBusVoltage_V(BusVoltageSensor);

    fmt_sendMsg((Top){
        .which_sub = Top_MotorTlm_tag,
        .sub = {
            .MotorTlm = {
                .motorState = motor->State,
                .speedRpm = ((int32_t)MCI_GetAvrgMecSpeedUnit(motor) * U_RPM) / SPEED_UNIT,
                .currentMa = motor->pFOCVars->Iqd.q,
                .busVoltageV = busVoltage,
                .uptime = uptime++,
            }}});
    break;
  }
  case 10:
  {
    uint32_t faultsCombined = MCI_GetFaultState(motor);
    uint16_t pastFaults = faultsCombined & 0xFFFF;
    uint16_t currentFaults = faultsCombined >> 16;

    PastFaultTlm pastFaultsMsg = {
        .DURATION = pastFaults & MC_DURATION,
        .OVER_VOLT = pastFaults & MC_OVER_VOLT,
        .UNDER_VOLT = pastFaults & MC_UNDER_VOLT,
        .OVER_TEMP = pastFaults & MC_OVER_TEMP,
        .START_UP = pastFaults & MC_START_UP,
        .SPEED_FDBK = pastFaults & MC_SPEED_FDBK,
        .OVER_CURR = pastFaults & MC_OVER_CURR,
        .SW_ERROR = pastFaults & MC_SW_ERROR,
        .DP_FAULT = pastFaults & MC_DP_FAULT,
    };
    fmt_sendMsg((Top){
        .which_sub = Top_PastFaultTlm_tag,
        .sub = {.PastFaultTlm = pastFaultsMsg},
    });

    CurrentFaultTlm CurrentFaultsMsg = {
        .DURATION = currentFaults & MC_DURATION,
        .OVER_VOLT = currentFaults & MC_OVER_VOLT,
        .UNDER_VOLT = currentFaults & MC_UNDER_VOLT,
        .OVER_TEMP = currentFaults & MC_OVER_TEMP,
        .START_UP = currentFaults & MC_START_UP,
        .SPEED_FDBK = currentFaults & MC_SPEED_FDBK,
        .OVER_CURR = currentFaults & MC_OVER_CURR,
        .SW_ERROR = currentFaults & MC_SW_ERROR,
        .DP_FAULT = currentFaults & MC_DP_FAULT,
    };
    fmt_sendMsg((Top){
        .which_sub = Top_CurrentFaultTlm_tag,
        .sub = {.CurrentFaultTlm = CurrentFaultsMsg},
    });
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
  bool ok = true;

  int32_t prevSpeed = (int32_t)MCI_GetMecSpeedRefUnit(motor) * U_RPM / SPEED_UNIT;

  MCI_ExecSpeedRamp(
      motor,
      (msg.speedRpm * SPEED_UNIT / U_RPM),
      abs(msg.speedRpm - prevSpeed) * 1000 / msg.rpmPerSec);

  // Clear faults that are there.
  if (motor->State == FAULT_OVER)
  {
    ok = ok && MCI_FaultAcknowledged(motor);
  }

  if (msg.run && motor->State == IDLE)
  {
    ok = ok && MCI_StartMotor(motor);
  }
  else if (!msg.run && motorRunning(motor))
  {
    ok = ok && MCI_StopMotor(motor);
  }
}

#define USE_Reset
void handleReset(Reset msg)
{
}

bool motorRunning(MCI_Handle_t *motor)
{
  switch (motor->State)
  {
  case ALIGNMENT:
  case CHARGE_BOOT_CAP:
  case OFFSET_CALIB:
  case START:
  case SWITCH_OVER:
  case RUN:
  {
    return true;
  }
  default:
    break;
  }
  return false;
}
