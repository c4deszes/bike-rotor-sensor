Sensor control
==============

Responsibilities are:

* Turning on the output stages after initialization
* Continuously monitoring the output stages and the sensor states
* Restarting channels when appropriate
* Turning off channels when requested for low power mode

MAX9621 & MAX9921
-----------------

These variants have no individual channel control but they automatically check each channel for
shorts and disconnects.

In this case the only action should be to disable timer capture for the channel.
