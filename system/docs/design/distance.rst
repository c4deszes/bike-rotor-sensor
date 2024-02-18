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
