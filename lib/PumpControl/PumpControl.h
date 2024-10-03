/**
 * @file PumpControl.h
 * @brief Header file for pump control functions in the PlantPal project.
 * 
 * This header file declares functions for controlling the pump's operation 
 * within the PlantPal project. It includes functionalities for pumping water 
 * at specified parameters and correcting soil capacitance by managing the pump.
 */

#ifndef PUMP_CONTROL_H
#define PUMP_CONTROL_H

/**
 * @brief Activates the pump for a specified duration.
 * 
 * This function initializes the pump control pin, turns on 
 * the pump, waits for the specified duration, and then 
 * turns off the pump. It also checks the pump status before 
 * and after activation to ensure proper operation.
 * 
 * @param pumpDuration The duration in milliseconds for which 
 *                     the pump should run.
 * @return true if the pump ran successfully and stopped as 
 *         expected, false otherwise.
 */
bool pumpWater(int pumpDuration);

/**
 * @brief Corrects soil capacitance by activating the pump.
 * 
 * This function reads the soil capacitance and adjusts the pump operation 
 * to achieve the desired soil conditions. It may involve multiple attempts 
 * to reach the target soil capacitance by activating the pump as necessary.
 */
void correctSoilCapacitive();

#endif