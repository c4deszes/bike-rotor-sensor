Electrical tests
================

Voltage supply
--------------

.. test:: Normal voltage operation
    :id: TEST_ELECTRICAL_VSUP_NORMAL
    :status: accepted

    1. Connect the ECU to the power supply
    2. Turn on the power supply at ``12V`` and ``500mA``
    3. Poll the speed value every ``100ms``
    4. Observe that the current drawn is below ``50mA``

.. test:: High voltage operation
    :id: TEST_ELECTRICAL_VSUP_HIGH
    :status: accepted

    1. Connect the ECU to the power supply
    2. Turn on the power supply at ``16V`` and ``500mA``
    3. Poll the speed value every ``100ms``
    4. Observe that the current drawn is below ``50mA``

.. test:: Low voltage operation
    :id: TEST_ELECTRICAL_VSUP_LOW
    :status: accepted

    1. Connect the ECU to the power supply
    2. Turn on the power supply at ``6V`` and ``500mA``
    3. Poll the speed value every ``100ms``
    4. Observe that the current drawn is below ``50mA``

Communication bus
-----------------

.. test:: Normal bus voltage
    :id: TEST_ELECTRICAL_COMM_SUP_NORMAL
    :status: accepted

    1. Connect the ECU to the power supply, Rcomm = 1kOhm
    2. Turn on the power supply at ``12V`` and ``500mA``
    3. Poll the speed value every ``100ms``
    4. Observe that the communication succeeds and that the baudrate stays constant

.. test:: High bus voltage
    :id: TEST_ELECTRICAL_COMM_SUP_HIGH
    :status: accepted

    1. Connect the ECU to the power supply, Rcomm = 1kOhm
    2. Turn on the power supply at ``16V`` and ``500mA``
    3. Poll the speed value every ``100ms``
    4. Observe that the communication succeeds and that the baudrate stays constant

.. test:: Low bus voltage
    :id: TEST_ELECTRICAL_COMM_SUP_LOW
    :status: accepted

    1. Connect the ECU to the power supply, Rcomm = 1kOhm
    2. Turn on the power supply at ``6V`` and ``500mA``
    3. Poll the speed value every ``100ms``
    4. Observe that the communication succeeds and that the baudrate stays constant

.. test:: Bus ground short
    :id: TEST_ELECTRICAL_COMM_GND_SHORT
    :status: accepted

    1. Connect the ECU to the power supply, Rcomm = 1kOhm
    2. Turn on the power supply at ``6V`` and ``500mA``
    3. Poll the speed value every ``100ms``
    4. At some point pull the bus to ground with ``0-100 Ohm`` resistor.
    5. Observe that the communication fails and that the ECU goes to sleep

.. test:: Bus voltage supply short
    :id: TEST_ELECTRICAL_COMM_VSUP_SHORT
    :status: accepted

    1. Connect the ECU to the power supply, Rcomm = 1kOhm
    2. Turn on the power supply at ``6V`` and ``500mA``
    3. Poll the speed value every ``100ms``
    4. At some point pull the bus to Vsup with ``0-100 Ohm`` resistor.
    5. Observe that the communication fails and that the ECU goes to sleep

Sensor output
-------------