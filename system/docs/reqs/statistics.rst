Ride statistics requirements
============================

Ride statistics are meant to capture user performance but more importantly they serve as the basis
for bicycle maintenance schedule.

Statistics are captured once the ride begins and they're saved as the ride ends.

.. req-sys:: Ride statistics
    :id: REQ_SYS_RIDE_STATISTICS
    :status: draft

    The device shall track the following signals during the ride:

    - Duration (sec)
    - Top speed (km/h)
    - Average speed (km/h)
    - Distance (km)
    - Total elevation (m)
    - Total descent (m)

Detailed statistics
-------------------

.. req-sys:: Ride duration
    :id: REQ_SYS_RIDE_DURATION
    :status: draft

    The device shall keep track of the duration of the ride.

.. req-sys:: Ride distance
    :id: REQ_SYS_RIDE_DISTANCE
    :status: draft

    The device shall keep track of the distance rode, distance measurement shall start once the
    ride begins.

    The distance shall be reported in meters resolution. This distance is accompanied by an error
    flag which shall be set in case of the following:

    - Both wheel sensors report failures, in this case if the sensor failure lasted for more than
      1 minute the distance error shall be on for the remainder of the ride, if failures disappear
      then the 1 minute counter shall be decremented
    - The distance measured on the two wheels differ by more than 250m, if the difference goes
      below 100m then the error flag shall be cleared

.. req-sys:: Ride elevation and descent
    :id: REQ_SYS_RIDE_ELEVATION_DESCENT
    :status: draft

    The device shall keep track of changes in elevation, measurement shall start once the ride
    begins.

    The elevation and descent values shall be reported in meters and both represent the amount of
    elevation or descent experienced during the ride. So if the rider climbs 100m and then descends
    down 20m then the elevation should be 100m and the descent is 20m.

    Climbs and descents only count when above 5% grade, meaning at least 5m of climb over 100m of
    distance travelled. The evaluation of this shall be continuous, 

.. req-sys:: Maximum and average speed
    :id: REQ_SYS_RIDE_MAX_AND_AVG_SPEED
    :status: draft

    The device shall keep track of the average and maximum speeds during the ride, the speed data
    shall only be accumulated into these statistics if the speed status is not in an error state.

.. req-sys:: Resetting ride statistics
    :id: REQ_SYS_RIDE_STAT_RESET
    :status: draft

    The device shall provide means to reset the ride statistics, even without resetting the ride.
