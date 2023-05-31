Communication
=============

.. jupyter-execute::
    :hide-code:

    %config InlineBackend.figure_format = 'svg'
    import schemdraw
    from schemdraw import elements as elm
    from schemdraw import flow
    import numpy as np
    from matplotlib import pyplot
    from IPython.display import Latex

Transceiver
-----------

:download:`MCP2021 <../datasheets/MCP2021A.pdf>` is a LIN Transceiver with voltage regulator
builtin, it can provide ``3.3V`` or ``5V`` output depending on the part.

In this design the regulator can be used to power the
:ref:`controller <design/controller:Controller>`, :ref:`controller <design/output_stage:Output stage>`
and the :ref:`sensor <design/sensor:Sensor>`

Filter
------

.. jupyter-execute::
    :hide-code:

    Latex(f'C_{{LIN}} = 220pF\n')

Schematic
---------

`To be done, after design decision`
