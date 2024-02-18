Measurement
===========

.. jupyter-execute::
    :hide-code:

    %config InlineBackend.figure_format = 'svg'
    import numpy as np
    from matplotlib import pyplot
    from IPython.display import Latex
    from prettytable import PrettyTable

Speed
------------

.. req-drs:: Period measurement
    :id: DRS_SYS_PERIOD_MEASUREMENT
    :status: draft
    :satisfies: REQ_SYS_SPEED_RANGE

    To measure the speed the software uses a timer to measure time elapsed between full rotations,
    given a wheel diameter of ``30"`` for the given speed the following period and frequency values can
    be expected.

    Resolution is the minimum distinguishable period change to achieve the ``0.1 km/h`` resolution
    requirement.

    .. jupyter-execute::
        :hide-code:

        resolution = 0.1
        poles = 1
        speed_values = [1, 3, 8, 15, 20, 30, 40, 50, 60, 70]
        wheel_diameter = (30 * 2.54) / 100  # wheel diameter in meters
        wheel_circumference = wheel_diameter * 3.141592

        def calculate(speed, poles, circumference):
            return 1 / ((speed / 3.6 / circumference) * poles)

        table = PrettyTable()
        table.field_names = ["Speed", "Period", "Frequency", "Resolution"]
        for speed in speed_values:
            period1 = calculate(speed, poles, wheel_circumference)
            period2 = calculate(speed + 0.1, poles, wheel_circumference)
            frequency = 1 / period1
            table.add_row([f"{speed} km/h", f"{period1*1000:.03f} ms", f"{frequency} Hz", f"{(period1-period2)*1000:.04f} ms"])

        print(table)

    Based on these values the timer resolution shall be ``1us`` with a maximum measured period of
    ``9s``, anything above this shall be considered as ``0 km/h``.

Distance
--------

.. req-drs:: Distance measurement
    :id: DRS_SYS_DISTANCE_MEASUREMENT
    :status: draft
    :satisfies: REQ_SYS_DISTANCE_RANGE

    To measure ``200km`` of distance at the stricter ``1m`` resolution requirement the number of
    increments the software needs to be able to record is ``200000``.

    Therefore the distance has to be stored as ``24 bit`` or more likely ``32 bit value``.

Braking
-------

.. req-drs:: Braking characteristics
    :id: DRS_SYS_BRAKING_CHARACTERISTICS
    :status: draft
    :satisfies: REQ_SYS_BRAKE_DETECTION

    Starting with an average commuting case on road/cycle lane with a speed of ``30km/h``, a riding
    weight of ``110kg`` and a constant braking power of ``300W``.

    The time required to reach full stop is determined by the following equation.

    .. jupyter-execute::
        :hide-code:

        Latex('\\frac{1}{2}mv^{2}=P_{braking}*t_{braking}')

    .. jupyter-execute::
        :hide-code:

        mass = 110
        speed = 30 / 3.6
        braking_power = 300
        braking_time = (1/2 * mass * speed * speed) / braking_power
        braking_distance = (speed * speed) / (4 * (braking_power / mass))

        Latex('\\begin{gathered}'
              f't_{{braking}}={braking_time:.03f} s\\\\'
              f'd_{{braking}}={braking_distance:.03f} m'
              '\end{gathered}')

    Given a car that's following ``1.5 seconds`` behind at a speed of ``35km/h`` with a weight of
    ``2000kg``, and a peak deceleration of ``5m/s^2 (0.5g)`` the distance relative to the bicycle
    would be the following under the assumption that braking is detected within ``500ms``:

    .. jupyter-execute::
        :hide-code:

        driver_reaction_time = 0.3
        separation_distance = 1         # Separation distance in seconds (speed dependent)
        car_mass = 2000                 # Car mass in kilograms (not used)
        car_speed = 30 / 3.6
        car_deceleration = 5            # Car maximum deceleration in m/s^2
        car_distance = car_speed * separation_distance
        car_final_distance = -(car_speed*car_speed) / (2 * -car_deceleration) - car_distance - braking_distance + car_speed * driver_reaction_time
        brake_indication_time = ((car_speed*car_speed) / (2 * -car_deceleration) + car_distance + braking_distance - car_speed * driver_reaction_time) / car_speed

        Latex('\\begin{gathered}'
              f'd_{{car}}={car_final_distance:.03f} m'
              '\end{gathered}')

    With this equation the amount of time available to indicate braking is calculated as follows,
    this assumes that the indication time simply adds to the reaction time where the driver starts
    braking and this time brings the car's front to the bike's rear exactly.

    .. jupyter-execute::
        :hide-code:

        Latex('\\begin{gathered}'
              f't_{{indication}}={brake_indication_time * 1000:.02f} ms'
              '\end{gathered}')

    Based on these values the device should be able to detect and report braking within ``100ms``.

References
----------

https://bicycles.stackexchange.com/questions/72863/what-are-the-typical-range-of-maximum-braking-friction-forces

https://www.youtube.com/watch?v=io5m32G1ATE&t=123s
