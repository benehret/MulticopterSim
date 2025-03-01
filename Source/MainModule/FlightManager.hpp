/*
 * Abstract, threaded flight-management class for MulticopterSim
 *
 * Copyright (C) 2019 Simon D. Levy
 *
 * MIT License
 */

#pragma once

#include "Dynamics.hpp"
#include "ThreadedManager.hpp"

class FFlightManager : public FThreadedManager {

    private:

        // Current motor values from PID controller
        double * _motorvals = NULL; 
        
        // For computing deltaT
        double   _previousTime = 0;

        bool _running = false;

        /**
         * Flight-control method running repeatedly on its own thread.  
         * Override this method to implement your own flight controller.
         *
         * @param time current time in seconds (input)
         * @param motorvals motor values returned by your controller (output)
         *
         */
        virtual void getMotors(const double time, double * motorvals)  = 0;
        
    protected:

        uint8_t _nmotors = 0;

        Dynamics * _dynamics = NULL;

        // Constructor, called main thread
        FFlightManager(Dynamics * dynamics) 
            : FThreadedManager()
        {
            // Constant
            _nmotors = dynamics->motorCount();

            // Allocate array for motor values
            _motorvals = new double[_nmotors];

            // Store dynamics for performTask()
            _dynamics = dynamics;

            // For periodic update
            _previousTime = 0;

            _running = true;
        }

        // Called repeatedly on worker thread to compute dynamics and run flight controller (PID)
        void performTask(double currentTime)
        {
            if (!_running) return;

            // Compute time deltay in seconds
			double dt = currentTime - _previousTime;

            // Send current motor values to dynamics
            _dynamics->setMotors(_motorvals);

            // Update dynamics
            _dynamics->update(dt);

            // PID controller: update the flight manager (e.g., HackflightManager) with
            // the dynamics state, getting back the motor values
            this->getMotors(currentTime, _motorvals);

            // Track previous time for deltaT
            _previousTime = currentTime;
        }


    public:

        static const uint8_t MAX_MOTORS = 16;

        ~FFlightManager(void)
        {
        }

        // Called by VehiclePawn::Tick() method to propeller animation/sound (motorvals)
        void getMotorValues(float * motorvals)
        {
            // Get motor values for propeller animation / motor sound
            for (uint8_t j=0; j<_nmotors; ++j) {
                motorvals[j] = _motorvals[j];
            }
        }

        void stop(void)
        {
            _running = false;
        }

}; // class FFlightManager
