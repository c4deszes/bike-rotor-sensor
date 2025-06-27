Ride logs
=========

At the beginning of each ride the software initializes a ride record, it contains the following:

* Identifier (2), as a unique identifier of the ride, with every ride it counts up, starts at 1
* Duration (2), length of the ride in seconds
* Distance (3), ride length in meters
* Top speed (2), recorded top speed in 0.1km/h
* Average speed (2), record average speed in 0.1km/h
* Total elevation (2), elevation in meters
* Total descent (2), descent in meters
* Reserved (1), reserved for future use
* Indications (4), flags indicating errors or changes

At the end of the ride the record is stored in a container.

A container contains the following:

* Write count (2)
* Last ride index (1)
* Records (25 * record size)
* Checksum (4)

At the end of every ride the container is stored on 2 rows. The are two container locations, the one
that's written is selected as follows:

* if both containers are valid then the one with the lower write counter is written
* if one container is invalid then that will be written over
* if neither container is valid then the first container is written

The two containers allow for a backup, if the write failed because of say power failures then it's
only missing one ride. It also helps with flash wear leveling.

Indications
-----------

* QNH unchanged (1), indicates that the QNH was not setup by the body computer
* Configuration changed (1), indicates that the body computer has changed the device configurations
* Reserved (6)



* Front speed error (1), indicates errors of the front speed sensor
* Rear speed error (1)
* Cadence sensor error (1)
* IMU sensor error (1)
* Pressure sensor error (1)
* Temperature sensor error (1)
* Distance error (1)
* Reserved (1)

Process
-------

Upon on initialization the component reads the two containers, based on their checksum and write
counter it determines which container's data to use, and which one to override.

As a ride is ended a record is pushed into the temporary container. As it's pushed the ride index
within the container is checked, if it's not been reset by the UDS command that dumps records then
eventually the index overflows. In this case the record's overflow flag is set.

The container is normally only saved when going to sleep or idle mode.
