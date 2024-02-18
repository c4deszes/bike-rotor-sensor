Speed measurement
=================

.. jupyter-execute::
    :hide-code:

    %config InlineBackend.figure_format = 'svg'
    import schemdraw
    from schemdraw import elements as elm
    from schemdraw import flow
    import numpy as np
    from matplotlib import pyplot
    from IPython.display import Latex
    from prettytable import PrettyTable

    def calculate(speed, poles, circumference):
        return 1 / ((speed / 3.6 / circumference) * poles)

Setup
-----

.. jupyter-execute::
    :hide-code:

    wheel_diameter = (30 * 2.54) / 100  # wheel diameter in meters
    wheel_circumference = wheel_diameter * 3.141592

Period range
------------

.. jupyter-execute::
    :hide-code:

    speed_values = [1, 3, 8, 15, 20, 30, 40, 50, 60, 70]
    pole_count = [1]

    table = PrettyTable()
    table.field_names = ["Speed / Poles"] + pole_count
    for speed in speed_values:
        periods = []
        for poles in pole_count:
            period = calculate(speed, poles, wheel_circumference)
            periods.append(f"{period * 1000:.04f} ms")
        table.add_row([f"{speed} km/h"] + periods)

    print(table)

Frequency range
---------------

.. jupyter-execute::
    :hide-code:

    table = PrettyTable()
    table.field_names = ["Speed / Poles"] + pole_count
    for speed in speed_values:
        frequencies = []
        for poles in pole_count:
            frequency = 1 / calculate(speed, poles, wheel_circumference)
            frequencies.append(f"{frequency} Hz")
        table.add_row([f"{speed} km/h"] + frequencies)

    print(table)

Resolution
----------

.. jupyter-execute::
    :hide-code:

    table = PrettyTable()
    table.field_names = ["Speed / Poles"] + pole_count
    for speed in speed_values:
        periods = []
        for poles in pole_count:
            period1 = calculate(speed, poles, wheel_circumference)
            period2 = calculate(speed + 0.1, poles, wheel_circumference)
            periods.append(f"{(period1-period2) * 1000:.04f} ms")
        table.add_row([f"{speed} km/h"] + periods)

    print(table)

Distance
--------

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

iTPMS
-----

.. jupyter-execute::
    :hide-code:

    wheel_diameter_delta = 0.005 * 2
    deflated_circumference = (wheel_diameter - wheel_diameter_delta) * 3.141592

    table = PrettyTable()
    table.field_names = ["Speed / Poles"] + pole_count

    for speed in speed_values:
        periods = []
        for poles in pole_count:
            difference = calculate(speed, poles, wheel_circumference) - calculate(speed, poles, deflated_circumference)
            periods.append(f"{difference * 1000:.04f} ms")
        table.add_row([f"{speed} km/h"] + periods)

    print(table)
