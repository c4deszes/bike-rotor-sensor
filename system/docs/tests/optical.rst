Optical performance tests
=========================

.. test:: Dust buildup
    :id: TEST_OPTICAL_DUST_BUILDUP
    :status: accepted

    0. Cover the sensor with dust in a horizontal position, then shake down the excess
    1. Connect the ECU to the power supply at temperature ``20°C``
    2. Turn on the power supply at ``12V`` and ``500mA``
    3. Poll the speed value every ``100ms``

.. test:: Liquid buildup
    :id: TEST_OPTICAL_LIQUID_BUILDUP
    :status: accepted

    0. Cover the sensor with water in a horizontal position, let the excess drip down
    1. Connect the ECU to the power supply at temperature ``20°C``
    2. Turn on the power supply at ``12V`` and ``500mA``
    3. Poll the speed value every ``100ms``

.. test:: IR interference
    :id: TEST_OPTICAL_IR_INTERFERENCE
    :status: accepted

    0. Setup the measurement in a sun lit environment with the sensor pointing directly at the sun
    1. Connect the ECU to the power supply at temperature ``20°C``
    2. Turn on the power supply at ``12V`` and ``500mA``
    3. Poll the speed value every ``100ms``
