# NEED.A.HAND
Important files for the robotic prosthesis hand project




Assemble following the image:

<img src="https://github.com/user-attachments/assets/cc970b68-e54f-47c8-92f3-b53f8dc8a22a" />



## WARNING: ASSEMBLY
- **Make sure all grounds are connected to each other** (Arduino GND, HC-08, power source and servos) or **it will burn**
- Value of resistors for the voltage divider is not important, as long as they are both the same. The voltage divider limits the voltage to the RX pin of the HC-08
- As written in the code, pins 8-11 are for servo control, and pins 6,7 are for BLE communication
- The exact position of wires is not that relevant
- The power wires for servos are put in the other side of the breadboard for clarity, but you can put them on the other side too
- **There might be mistakes in the diagram**, use your knowledge to assess before powering on


## BLE HC-08
The HC-08 uses BLE (Bluetooth Lightweight). Everything should be set up and ready to be used. In case adjustments need to be made:

1. Wire everything up, and power the Arduino
2. BLE should start blinking blue. This means it is now reading commands from Arduino Serial. You can write **without line ending or new line**:
   - AT -> Will send "OK"
   - AT+RX -> Will send the current settings
   - More commands in [this guide](https://www.hc01.com/downloads/HC-08A%20version%20english%20datasheet.pdf)
3. After everything is set up, you can connect via *bluetooth terminal* from a phone
