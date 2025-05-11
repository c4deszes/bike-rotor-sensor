Logging requirements
====================

.. req-sys:: Ride logs
    :id: REQ_SYS_RIDE_LOGS
    :status: draft

    The device shall track the rides completed and some parameters associated with each ride:

    - Identifier (2)
    - Duration (2)
    - Distance (3/4)
    - Top Speed (2)
    - Average Speed (2)
    - Total elevation (2)
    - Total descent (2)
    - iTPMS indication
    - Sensor error indications (front, rear, crank, IMU, barometer)

    The device shall save the latest rides in non-volatile memory, the save should occur as the
    device is sent to sleep or shutdown.
