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

AP22804
-------

This variant has individual channel control but it also automatically detects shorts.
No open detection.

In this case we should disable timer capture for the shorted channel.

The question is do we leave the open channels enabled? How do we detect an open channel?
One solution might be that we pull the input high at the sensor end and if it's low
for a while we assume it's open.
