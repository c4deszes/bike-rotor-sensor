Output stage design
===================

The output stage consists of 3 parts:

* Physical driver (Integrated Circuit)
* Sensor driver (Optical, Hall, etc.)
* Sensor control (described in :ref:`design/sensor_control`)

Physical layer
--------------

Responsibilities are turning on and off the two channels, checking for errors on channels, like
short circuits and disconnected sensors. The common part of this component configures the tiemrs
for each channel and connects their event input to the generators.

Sensor layer
------------

Responsibilities are handling the input pulses, determining rotational direction if possible and
detecting sensor specific warnings and errors. The regular speed pulses are forwarded to speed
monitoring.
