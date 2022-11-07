# Stop-Watch-System

Description:

    1. Used ATmega32 Microcontroller with frequency 1Mhz.

    2. Configured Timer1 in ATmega16 with CTC mode to count the Stop Watch time.

    3. Used six Common Anode 7-segments.

    4. Connected the six 7-segments in the project using the multiplexed technique. You should use one 7447 decoder for all 7-segments and control the enable/disable for each 7-segement using a NPN BJT transistor connect to one of the MCU pins.

    5. We can connected more than one 7-segment display by using the Multiplexing method. In this method, at a time one 7-segment display is driven by the Microcontroller and the rest are OFF. It keeps switching the displays using transistors. Due to the persistence of vision, it appears as a normal display.

    6. Configured External Interrupt INT0 with falling edge. Connect a push button with the internal pull-up resistor. If a falling edge detected the Stop Watch time should be reset.

    7. Configured External Interrupt INT1 with raising edge. Connect a push button with the external pull-down resistor. If a raising edge detected the Stop Watch time should be paused.

    8. Configured External Interrupt INT2 with falling edge. Connect a push button with the internal pull-up resistor. If a falling edge detected the Stop Watch time should be resumed.
