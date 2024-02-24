Power conditioning
==================

.. jupyter-execute::
    :hide-code:

    %config InlineBackend.figure_format = 'svg'
    import schemdraw
    from schemdraw import elements as elm
    from schemdraw import flow
    import numpy as np
    from matplotlib import pyplot
    from IPython.display import Latex
    from UliEngineering.Electronics.Resistors import normalize_numeric


Reverse polarity protection
---------------------------

A Schottky diode can be used to prevent reverse polarity from damaging components.

.. hw-drs:: RPP Schotkky diode component list
    :id: HW_DRS_RPP_SCHOTTKY_DIODE
    :status: accepted
    :satisfies: ext_CRS_LOCKOUT_ACTUATION_TIME

    :download:`B140-13-F <../datasheets/B120-160B>` from Diodes Inc. is selected as the RPP diode.
    It has a breakdown voltage of 40V and a maximum continuous current of 1A.

Calculation
~~~~~~~~~~~

.. jupyter-execute::
    :hide-code:

    v_forward = '0.55V'
    i_max = '150mA'
    i_idle = '50mA'
    t_per_watt = '20°C'
    t_operational = '50°C'

    P_max = normalize_numeric(v_forward) * normalize_numeric(i_max)
    P_idle = normalize_numeric(v_forward) * normalize_numeric(i_idle)

    T_max = normalize_numeric(t_per_watt) * P_max + normalize_numeric(t_operational)

The maximum power dissipated in the diode can be calculated and based on this worst case load the
diode temperature can be estimated at the maximum operating temperature.

.. jupyter-execute::
    :hide-code:

    Latex(f"P_{{max}} = {P_max:.02f}W, T_{{max}} ≈ {T_max}°C")

Transient protection
--------------------

A TVS diode can be used to prevent temporary overvoltage conditions on the power input.

Schematic
---------

.. jupyter-execute::
    :hide-code:

    with schemdraw.Drawing() as d:
        d += (tvs := elm.Diac()).down().label('$D_{TVS}$')
        d += elm.Ground().at(tvs.end)
        d += elm.Line().left().length(d.unit / 4).label('$V_{bat}$', 'left').at(tvs.start)
        d += elm.Dot().at(tvs.start)
        d += elm.Schottky().right().label('$D_{RPP}$').at(tvs.start).label('$V_{out}$', 'right')
