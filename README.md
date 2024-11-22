data-acquisition-system :-

This project involves designing and implementing a Data Acquisition System (DAS) on the LPC2129 microcontroller, which collects data from various sensors (temperature sensor, LDR, potentiometer) and logs it with
real-time timestamps using an RTC (Real-Time Clock). The system uses ADC (Analog-to-Digital Conversion), SPI, and I2C.

Key Components and Modules
1. Microcontroller: LPC2129
2. Sensors : Temperature Sensor (e.g., LM35):
3. Light Dependent Resistor (LDR):Detects light intensity as an analog signal. Input to ADC.
4. Potentiometer: Used to measure adjustable voltage for calibration or testing. Input to ADC.
5. Real-Time Clock (RTC, e.g., DS1307) Communicates via I2C. Provides accurate timestamps for sensor data logging.


ADC (Analog-to-Digital Converter): Converts analog signals from sensors to digital values.
SPI (Serial Peripheral Interface): Used to interface with external storage devices (e.g., EEPROM or SD card).
I2C (Inter-Integrated Circuit): Interfaces the RTC and other peripherals.

5. Output Devices
16x2 LCD Display:
Displays real-time sensor data and timestamps.
UART (Optional):
Sends data to a PC for monitoring via serial communication.
