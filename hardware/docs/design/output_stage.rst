Output stage
============

.. jupyter-execute::
    :hide-code:

    %config InlineBackend.figure_format = 'svg'
    import schemdraw
    from schemdraw import elements as elm
    from schemdraw import flow
    import numpy as np
    from matplotlib import pyplot
    from IPython.display import Latex
    from UliEngineering.Electronics.Resistors import power_dissipated_in_resistor_by_current, normalize_numeric

3-wire option
-------------

Voltage divider
~~~~~~~~~~~~~~~

:download:`AP22802 <../datasheets/AP22802.pdf>` is a 5V device, therefore the overcurrent flag and
the input from the sensor needs to be level shifted down to 3.3V.

.. jupyter-execute::
    :hide-code:

    v_sup = '5V'
    r_top = '1kΩ'
    r_bot = '1kΩ'
    v_out = normalize_numeric(v_sup) * normalize_numeric(r_bot) / (normalize_numeric(r_top) + normalize_numeric(r_bot))

    Latex("\\begin{gathered}"
          f"V_{{supply}} = {v_sup}\\\\"
          f"R_{{top}} = {r_top}\\\\"
          f"R_{{bottom}} = {r_bot}\\\\"
          f"V_{{out}} = V_{{supply}} * \\frac{{R_{{bottom}}}}{{R_{{top}}+R_{{bottom}}}} = {v_out:.03f}V\\\\"
          "\end{gathered}")


Input filtering
~~~~~~~~~~~~~~~

High frequency glitches are filtered by an RC low pass filter, transients are suppressed using TVS
diodes on the supply and input lines.

.. jupyter-execute::
    :hide-code:

    c_in = '22nF'
    r_in = '1kΩ'
    f_cutoff = 1 / (2 * 3.141592 * (normalize_numeric(c_in) * normalize_numeric(r_in)))

    Latex("\\begin{gathered}"
          f"C_{{in}} = {c_in}\\\\"
          f"R_{{in}} = {r_in}\\\\"
          f"f_{{cutoff}} = \\frac{{1}}{{2 \\pi RC}} = {f_cutoff / 1000:.03f}kHz\\\\"
          "\end{gathered}")

2-wire option
-------------

Chip selection
~~~~~~~~~~~~~~

:download:`MAX9621 <../datasheets/MAX9621.pdf>` and :download:`MAX9921 <../datasheets/MAX9921.pdf>`
are the candidates for 2-wire output stages. They both supply the Hall-effect sensor from `Vbat` and
measure the current consumption of  using which they output a digital signal.

The two ICs can be retrofitted as the output channels and high voltage pins are identical, the
differences are at the low voltage side, some pins here are inputs in one chip while they are
outputs in the other but this can be compensated in software.

.. list-table:: Pinout
    :header-rows: 1

    * - Pin number
      - MAX9621 Pin
      - MAX9921 Pin
      - Note

    * - 1
      - VBat
      - VBat
      - 

    * - 2
      - Iset
      - Iset
      - 

    * - 3
      - Input 1
      - Input 1
      - 

    * - 4
      - Input 2
      - Input 2
      - 

    * - 5
      - Gnd
      - Gnd
      - 

    * - 6
      - Digital out 2 (output)
      - Digital out 2 (output)
      - 

    * - 7
      - Analog out 2 (output)
      - Digital out 2 (output)
      - In MAX9621 ADC peripheral might be used

    * - 8
      - Digital out 1 (output)
      - Error (output)
      - In MAX9921 interrupts may not be needed

    * - 9
      - Analog out 1 (output)
      - Output enable (input)
      - In MAX9921 ADC peripheral might be used

    * - 10
      - Sleep (input)
      - Diagnostics (input)
      - 

Hall-sensor selection
~~~~~~~~~~~~~~~~~~~~~

.. list-table:: Sensors
    :header-rows: 1

    * - Order code
      - Max. frequency
      - Direction detection
      - Protocol
      - Error detection
      - Technology

    * - TLE49421CHAMA2
      - 1 - 2500 Hz
      - Yes
      - PWM (current)
      - Airgap
      - Hall

    * - TLE5045ICR050HALA1
      - 0.03 - 3000 Hz
      - No
      - PWM (current)
      - Supply errors
      - iGMR

    * - TLE5046ICAKLRHALA1
      - 0- 3000 Hz
      - Yes
      - AK
      - Supply errors, air gap
      - iGMR
