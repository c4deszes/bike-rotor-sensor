Distance measurement
====================

.. jupyter-execute::
    :hide-code:

    %config InlineBackend.figure_format = 'svg'
    import numpy as np
    from matplotlib import pyplot
    from IPython.display import Latex
    from prettytable import PrettyTable

    pole_count = [1]
    
    wheel_diameter = (30 * 2.54) / 100  # wheel diameter in meters
    wheel_circumference = wheel_diameter * 3.141592

Maximum ride distance
---------------------

.. req-drs:: Distance measurement
    :id: DRS_SYS_DISTANCE_MEASUREMENT
    :status: draft
    :satisfies: REQ_SYS_DISTANCE_RANGE

    This table shows the different ride distances that can be measured given a pole configuration that
    effectively sets our resolution and the variable size we use.

    .. jupyter-execute::
        :hide-code:

        table = PrettyTable()
        table.field_names = ["Signal width / Poles"] + pole_count
        for max_value in [2**16, 2**24, 2**32]:
            values = []
            for poles in pole_count:
                max_dist = (max_value / poles) * wheel_circumference
                values.append(f"{max_dist/1000:.03f} km")
            table.add_row([f"{max_value} (int)"] + values)

        print(table)

    To measure ``200km`` of distance at the stricter ``1m`` resolution requirement the number of
    increments the software needs to be able to record is ``200000``.

    Therefore the distance has to be stored as ``24 bit`` or more likely ``32 bit value``.

State machine
-------------

The distance measurement is accompanied by a state value, the state can be used to determine whether
the distance value is reliable.

Initially the state is `N/A`, once the ride begins it transitions to `Ok`.

If at any point the distance measured for the two wheels go above a threshold then the state will be
set to `Error` and only go back to `Ok` if a lower threshold is crossed.

Sensors with an error status are ignored, for example if the front wheel reports error then the front
wheel's distance measurement is incremented as the rear wheel rotates.

If both sensors report errors for 60sec and the ride has started then the distance state will be
`PermanentError` for the remainder of the ride.

The returned distance no matter the state will be larger of the two wheel measurements
