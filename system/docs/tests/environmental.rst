Environmental tests
===================

Temperature
-----------

.. test:: Very low temperature
    :id: TEST_ENVIRONMENTAL_TEMP_VERY_LOW
    :status: accepted

    1. Connect the ECU to the power supply at temperature ``-10°C``
    2. Turn on the power supply at ``12V`` and ``500mA``
    3. Poll the speed value every ``100ms``

.. test:: Low temperature
    :id: TEST_ENVIRONMENTAL_TEMP_LOW
    :status: accepted

    1. Connect the ECU to the power supply at temperature ``0°C``
    2. Turn on the power supply at ``12V`` and ``500mA``
    3. Poll the speed value every ``100ms``

.. test:: Room temperature
    :id: TEST_ENVIRONMENTAL_TEMP_ROOM
    :status: accepted

    1. Connect the ECU to the power supply at temperature ``20°C``
    2. Turn on the power supply at ``12V`` and ``500mA``
    3. Poll the speed value every ``100ms``

.. test:: High temperature
    :id: TEST_ENVIRONMENTAL_TEMP_HIGH
    :status: accepted

    1. Connect the ECU to the power supply at temperature ``50°C``
    2. Turn on the power supply at ``12V`` and ``500mA``
    3. Poll the speed value every ``100ms``

Humidity
--------

.. test:: Low humidity
    :id: TEST_ENVIRONMENTAL_HUMIDITY_LOW
    :status: accepted

    1. Connect the ECU to the power supply at temperature ``20°C`` and ``20% RH``
    2. Turn on the power supply at ``12V`` and ``500mA``
    3. Poll the speed value every ``100ms``

.. test:: Normal humidity
    :id: TEST_ENVIRONMENTAL_HUMIDITY_NORMAL
    :status: accepted

    1. Connect the ECU to the power supply at temperature ``20°C`` and ``50% RH``
    2. Turn on the power supply at ``12V`` and ``500mA``
    3. Poll the speed value every ``100ms``

.. test:: High humidity
    :id: TEST_ENVIRONMENTAL_HUMIDITY_HIGH
    :status: accepted

    1. Connect the ECU to the power supply at temperature ``20°C`` and ``70% RH``
    2. Turn on the power supply at ``12V`` and ``500mA``
    3. Poll the speed value every ``100ms``


.. test:: Very high humidity
    :id: TEST_ENVIRONMENTAL_HUMIDITY_VERY_HIGH
    :status: accepted

    1. Connect the ECU to the power supply at temperature ``20°C`` and ``90% RH``
    2. Turn on the power supply at ``12V`` and ``500mA``
    3. Poll the speed value every ``100ms``

Ingress protection
------------------

.. test:: Water shower (sensor side)
    :id: TEST_ENVIRONMENTAL_INGRESS_WATER_SENSOR
    :status: accepted

    1. Connect the ECU to the power supply at temperature ``20°C``
    2. Turn on the power supply at ``12V`` and ``500mA``
    3. Poll the speed value every ``100ms``
    4. Apply light water shower aimed diagonally at the sensor and cover side

.. test:: Water shower (outside)
    :id: TEST_ENVIRONMENTAL_INGRESS_WATER_OUTSIDE
    :status: accepted

    1. Connect the ECU to the power supply at temperature ``20°C``
    2. Turn on the power supply at ``12V`` and ``500mA``
    3. Poll the speed value every ``100ms``
    4. Apply light water shower aimed diagonally at fork and cover side

.. test:: Water jet (front)
    :id: TEST_ENVIRONMENTAL_INGRESS_WATER_FRONT
    :status: accepted

    1. Connect the ECU to the power supply at temperature ``20°C``
    2. Turn on the power supply at ``12V`` and ``500mA``
    3. Poll the speed value every ``100ms``
    4. Apply light water shower aimed diagonally at the sensor and cover side
