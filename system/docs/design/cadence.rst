Cadence measurement
===================

.. jupyter-execute::
    :hide-code:

    %config InlineBackend.figure_format = 'svg'
    import numpy as np
    from matplotlib import pyplot
    from IPython.display import Latex
    from prettytable import PrettyTable

.. req-drs:: Cadence measurement
    :id: DRS_SYS_CADENCE_MEASUREMENT
    :status: draft

    The cadence value is expected in the range of 10-200RPM, this depends on the current speed and
    gear ratios.

    Assuming full power, so not coasting or pedaling slower than the bike's actual speed the
    cadence comes out to these values:

    .. jupyter-execute::
        :hide-code:

        speed_values = [1, 3, 8, 15, 20, 30, 40, 50, 60, 70]
        gear_range = [42/48, 42/41, 42/34, 42/28, 42/23, 42/20, 42/17, 42/15, 42/13, 42/11]
        wheel_circumference = 2.275

        table = PrettyTable()
        table.field_names = ["Speed / Gear"] + [f"{42/gear:.00f}" for gear in gear_range]
        for speed in speed_values:
            cadences = []
            for gear in gear_range:
                value = (((speed / 3.6) / wheel_circumference) * 60) / gear
                cadences.append(value)
            table.add_row([f"{speed} km/h"] + [f"{cadence:.00f} RPM" for cadence in cadences])

        print(table)
