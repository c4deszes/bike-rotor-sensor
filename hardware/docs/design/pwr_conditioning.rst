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

    :download:`SS34F <../datasheets/SS34F.pdf>`  from DC Components is selected as the RPP diode. It has
    a ``40V`` breakdown voltage and ``3A`` of maximum average forward current.

Calculation
~~~~~~~~~~~

.. jupyter-execute::
    :hide-code:

    v_forward = '0.55V'
    i_max = '2.0A'
    i_idle = '100mA'
    t_per_watt = '55°C'
    t_operational = '40°C'

    P_max = normalize_numeric(v_forward) * normalize_numeric(i_max)
    P_idle = normalize_numeric(v_forward) * normalize_numeric(i_idle)

    T_max = normalize_numeric(t_per_watt) * P_max + normalize_numeric(t_operational)

The maximum power dissipated in the diode can be calculated and based on this worst case load the
diode temperature can be estimated at ambient temperature

.. jupyter-execute::
    :hide-code:

    Latex(f"P_{{max}} = {P_max:.02f}W, T_{{max}} ≈ {T_max}°C")

Transient protection
--------------------

A TVS diode can be used to prevent temporary overvoltage conditions on the power input.

Regulator enable
----------------

Design 1
~~~~~~~~

A series P-channel MOSFET can be used to disable the power input to the
:ref:`power stage <design/pwr_stage:Power stage>`.

Design 2 (chosen)
~~~~~~~~~~~~~~~~~

A regulator that has builtin shutdown pin can be used, a direct connection from
:ref:`communication <design/communication:Communication>` enable output can be made, or in case the
transceiver there is able to provide power to the control portion then that can provide this enable
signal.

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
