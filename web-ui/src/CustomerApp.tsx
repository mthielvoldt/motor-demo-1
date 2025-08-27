// src/App.tsx
import React, { useEffect, useState } from "react";
import { setMessageHandler, sendMessage, BrokerAddress } from 'firment-ui';

enum MotorState {
  Stopped = 0,
  Running = 1,
  Error = 2,
}


interface FanState {
  motorState: number; // required by external module
  speedRpm: number;
}

interface MotorTlm {
  motorState: number;
  speedRpm: number;
}

function App() {
  const [state, setState] = useState<FanState>({
    motorState: MotorState.Stopped,
    speedRpm: 2000,
  });

  useEffect(() => {
    setMessageHandler("MotorTlm", (tlm: MotorTlm) => {
      setState((prev) => ({ ...prev, motorState: getCustomerMotorState(tlm) }));
    });
    setMessageHandler("PastFaultTlm", (tlm: {}) => (tlm));
  }, []);

  function sendCommand(run: boolean) {
    sendMessage("MotorCtl",
      {
        run,
        speedRpm: state.speedRpm,
        rpmPerSec: 500,
      },
      true
    );
  };


  function handleStart() {
    sendCommand(true);
  }
  function handleStop() {
    sendCommand(false);
  }
  function handleSpeedChange(e: React.ChangeEvent<HTMLInputElement>) {
    setState((prev) => ({ ...prev, speedRpm: Number(e.target.value) }));
  }
  function handleSpeedSelect() {
    sendCommand(state.motorState === MotorState.Running);
  }

  function getStatusLabel() {
    switch (state.motorState) {
      case MotorState.Running:
        return "Running";
      case MotorState.Stopped:
        return "Stopped";
      case MotorState.Error:
        return "Error";
      default:
        return "Unknown";
    }
  }
  function getCustomerMotorState(tlm: MotorTlm) {
    switch (tlm.motorState) {
      case 0: // IDLE
      case 8: // STOP
        return MotorState.Stopped
      case 2: // ALIGNMENT
      case 4: // START
      case 6: // RUN
      case 19:// SWITCH_OVER
      case 20:// WAIT_STOP_MOTOR
        return MotorState.Running
      default:
        return MotorState.Error
    }
  }

  return (
    <div className="p-6 flex flex-col gap-4 max-w-md mx-auto">
      <h1 className="text-xl font-bold">Fan Controller</h1>
      <BrokerAddress />

      <div className="flex gap-2">
        <button
          className="bg-green-500 text-white px-4 py-2 rounded"
          onClick={handleStart}
        >
          Start
        </button>
        <button
          className="bg-red-500 text-white px-4 py-2 rounded"
          onClick={handleStop}
        >
          Stop
        </button>
      </div>

      <div>
        <label className="block mb-2">Speed: {state.speedRpm}</label>
        <input
          type="range"
          min={600}
          max={4000}
          value={state.speedRpm}
          onChange={handleSpeedChange}
          onPointerUp={handleSpeedSelect}
          className="w-full"
        />
      </div>

      <div className="font-medium">
        Status: <span>{getStatusLabel()}</span>
      </div>
    </div>
  );
}

export default App;
