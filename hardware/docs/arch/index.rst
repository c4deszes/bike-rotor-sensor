Hardware architecture
=====================

Overview
--------

The hardware architecture is almost the same as the system architecture with the added detail of
some filtering in between blocks.

Detailed responsibilities
-------------------------

Power conditioning
~~~~~~~~~~~~~~~~~~

This block takes the nominal ``12V`` battery voltage and suppresses voltage spikes and makes sure
the polarity is correct.

Communication
~~~~~~~~~~~~~

Converts the LIN bus level to levels acceptable by the controlling logic, also has some responsibility
in controlling the device's state, going to sleep and waking up. This block also provides power to
the other components through the builtin voltage regulator.

Controller
~~~~~~~~~~

Takes signals from the communication line and controls the output stage according to the requirements.
Processes the data coming from the output stage and the IMU to provide the speed information.

Output stage
~~~~~~~~~~~~

Provides power to the speed sensors and conditions their output to be acceptable by the controller.

IMU
~~~

Measures acceleration and angular changes which is processed by the controller.

Variation points
----------------

Crystal
~~~~~~~

The crystal is optionally fitted.

Output stage
~~~~~~~~~~~~

* Option 1: MAX9621, Transceiver is 3.3V
* Option 2: MAX9921, Transceiver is 3.3V
* Option 3: AP22804AW5, Transceiver is 5.0V, 3.3V regulator is populated
