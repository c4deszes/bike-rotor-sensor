Output stage design
===================

The output stage comprises of 3 parts:

* Physical driver (Integrated Circuit)
* Sensor driver (Optical, Hall, etc.)
* Data processor

OSH PHY
-------

Responsibilities are turning on and off the two channels, checking for errors on channels, like
short circuits and disconnected sensors.

OSH Sensor
----------

Responsibilities are converting input pulse lengths into normalized speed (pulse per second) and
determining rotational direction if possible.

Data processor
--------------

Responsible for converting the pulse per second data into rotational speed, it forwards this data
to the communication module. It also handles output stage failures, restarting the channels.
