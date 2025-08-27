// src/App.tsx
import React, { useEffect, useState } from "react";
import { setMessageHandler, sendMessage, BrokerAddress } from 'firment-ui';
import './CustomerApp.css'

enum MotorState {
  Stopped = 0,
  Running = 1,
  Error = 2,
}


interface FanState {
  motorState: number; // required by external module
  speed: number;
}

interface MotorTlm {
  motorState: number;
  speedRpm: number;
}

function App() {
  const [state, setState] = useState<FanState>({
    motorState: MotorState.Stopped,
    speed: 2000,
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
        speedRpm: state.speed,
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
    setState((prev) => ({ ...prev, speed: Number(e.target.value) }));
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
      default:
        return "Error";
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
    <div className="app">
      <div className="card">
        <h1 className="title">Fan Controller</h1>
        <BrokerAddress />
        <div className="button-row">
          <button className="btn start" onClick={handleStart}>
            Start
          </button>
          <button className="btn stop" onClick={handleStop}>
            Stop
          </button>
        </div>

        <div className="slider-block">
          <label className="slider-label">Speed</label>
          <input
            type="range"
            min={800}
            max={4000}
            value={state.speed}
            onChange={handleSpeedChange}
            onPointerUp={handleSpeedSelect}
          />
          <div className="rpm">{state.speed} rpm</div>
        </div>

        <div className="status">
          Status:{" "}
          <span className={getStatusLabel().toLocaleLowerCase()}>
            {getStatusLabel()}
          </span>
        </div>
      </div>
    </div>
  );
}

export default App;
