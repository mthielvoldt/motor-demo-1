
/**
  ******************************************************************************
  * @file    mc_config_common.c
  * @author  Motor Control SDK Team,ST Microelectronics
  * @brief   Motor Control Subsystem components configuration and handler structures.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044,the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

//cstat -MISRAC2012-Rule-21.1
#include "main.h" //cstat !MISRAC2012-Rule-21.1
//cstat +MISRAC2012-Rule-21.1
#include "mc_type.h"
#include "parameters_conversion.h"
#include "mc_parameters.h"
#include "mc_config_common.h"

/* USER CODE BEGIN Additional include */

/* USER CODE END Additional include */

/* USER CODE BEGIN Additional define */

/* USER CODE END Additional define */

/**
  * @brief  SpeedNPosition sensor parameters Motor 1 - Base Class.
  */
VirtualSpeedSensor_Handle_t VirtualSpeedSensorM1 =
{

  ._Super =
  {
    .bElToMecRatio             = POLE_PAIR_NUM,
    .hMaxReliableMecSpeedUnit  = (uint16_t)(1.15*MAX_APPLICATION_SPEED_UNIT),
    .hMinReliableMecSpeedUnit  = (uint16_t)(MIN_APPLICATION_SPEED_UNIT),
    .bMaximumSpeedErrorsNumber = M1_SS_MEAS_ERRORS_BEFORE_FAULTS,
    .hMaxReliableMecAccelUnitP = 65535,
    .hMeasurementFrequency     = TF_REGULATION_RATE_SCALED,
    .DPPConvFactor             = DPP_CONV_FACTOR,
  },

  .hSpeedSamplingFreqHz        = MEDIUM_FREQUENCY_TASK_RATE,
  .hTransitionSteps            = (int16_t)((TF_REGULATION_RATE * TRANSITION_DURATION) / 1000.0),
};

/**
  * temperature sensor parameters Motor 1.
  */
RegConv_t TempRegConv_M1 =
{
  .regADC                = ADC1,
  .channel               = MC_ADC_CHANNEL_5,
  .samplingTime          = M1_TEMP_SAMPLING_TIME,
  .data                  = 0
};

NTC_Handle_t TempSensor_M1 =
{
  .bSensorType             = REAL_SENSOR,
  .hOverTempThreshold      = (uint16_t)(OV_TEMPERATURE_THRESHOLD_d),
  .hOverTempDeactThreshold = (uint16_t)(OV_TEMPERATURE_THRESHOLD_d - OV_TEMPERATURE_HYSTERESIS_d),
  .hSensitivity            = (int16_t)(ADC_REFERENCE_VOLTAGE/dV_dT),
  .wV0                     = (uint16_t)((V0_V * 65536) / ADC_REFERENCE_VOLTAGE),
  .hT0                     = T0_C,
};

/**
  * Bus voltage sensor parameters Motor 1.
  */
RegConv_t VbusRegConv_M1 =
{
  .regADC                   = ADC1,
  .channel                  = MC_ADC_CHANNEL_1,
  .samplingTime             = M1_VBUS_SAMPLING_TIME,
  .data                     = 1 + (uint16_t)((NOMINAL_BUS_VOLTAGE_V * 65536) / (ADC_REFERENCE_VOLTAGE / VBUS_PARTITIONING_FACTOR))
};

RDivider_Handle_t BusVoltageSensor_M1 =
{
  ._Super =
  {
    .SensorType               = REAL_SENSOR,
    .ConversionFactor         = (uint16_t)(ADC_REFERENCE_VOLTAGE / VBUS_PARTITIONING_FACTOR),
  },

  .OverVoltageThreshold       = OVERVOLTAGE_THRESHOLD_d,
  .OverVoltageThresholdLow    = OVERVOLTAGE_THRESHOLD_d,
  .OverVoltageHysteresisUpDir = true,
  .UnderVoltageThreshold      =  UNDERVOLTAGE_THRESHOLD_d,
};

/** @brief */
uint16_t SpeedPotentiometer_ValuesBuffer_M1[ 1 << POTENTIOMETER_LPF_BANDWIDTH_POW2_M1 ];

/*
 * Speed Potentiometer instance for motor 1
 */
RegConv_t PotRegConv_M1 =
{
  .regADC              = ADC1,
  .channel             = MC_ADC_CHANNEL_11,
  .samplingTime        = POTENTIOMETER_ADC_SAMPLING_TIME_M1,
  .data                = 0
};

SpeedPotentiometer_Handle_t SpeedPotentiometer_M1 =
{
  .Pot =
  {
    .LPFilterBandwidthPOW2 = POTENTIOMETER_LPF_BANDWIDTH_POW2_M1,
    .PotMeasArray          = (uint16_t *)SpeedPotentiometer_ValuesBuffer_M1
  },

  .pMCI                    = & Mci[0],
  .RampSlope               = (uint32_t)(POTENTIOMETER_RAMP_SLOPE_M1),
  .ConversionFactor        = (uint16_t)((65536.0) / ((POTENTIOMETER_MAX_SPEED_M1) - (POTENTIOMETER_MIN_SPEED_M1))),
  .SpeedAdjustmentRange    = (uint16_t)(POTENTIOMETER_SPEED_ADJUSTMENT_RANGE_M1),
  .MinimumSpeed            = (uint16_t)(POTENTIOMETER_MIN_SPEED_M1),
};

PWMC_Handle_t *pwmcHandle[NBR_OF_MOTORS];

/* USER CODE BEGIN Additional configuration */

/* USER CODE END Additional configuration */

/******************* (C) COPYRIGHT 2025 STMicroelectronics *****END OF FILE****/

