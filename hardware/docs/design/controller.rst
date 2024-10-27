Controller
==========

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

    def ap2127_output(rtop, rbot):
        return 0.8 * (1 + normalize_numeric(rtop) / normalize_numeric(rbot))

Voltage regulator
-----------------

:download:`AP2127 <../datasheets/AP2127.pdf>` is used to regulate the voltage from the ``5V`` used
by the output stage to ``3.3V`` needed for the microcontroller.

In case the :ref:`communication transceiver <design/communication:Transceiver>` is ``MCP2021x-330E``
then this regulator is not needed.

Calculation
~~~~~~~~~~~

.. jupyter-execute::
    :hide-code:

    rtop = '4.7kΩ'
    rbot = '1.5kΩ'

.. jupyter-execute::
    :hide-code:

    output_voltage = ap2127_output(rtop, rbot)
    Latex("\\begin{gathered}"
          f"R_{{top}} = {rtop}\\\\"
          f"R_{{bottom}} = {rbot}\\\\"
          f"V_{{ref}} = 0.8V\\\\"
          f"V_{{out}} = V_{{ref}} (1 + \\frac{{R_{{top}}}}{{R_{{bottom}}}}) = {output_voltage:.03f}V"
          "\end{gathered}")

Schematic
~~~~~~~~~

.. jupyter-execute::
    :hide-code:

    with schemdraw.Drawing() as d:
        AP2127 = elm.Ic(pins=[
                        elm.IcPin(name='EN', pin='3', side='left'),
                        elm.IcPin(name='VIN', pin='1', side='left'),
                        elm.IcPin(name='GND', pin='2', side='bottom'),
                        elm.IcPin(name='ADJ', pin='4', side='right'),
                        elm.IcPin(name='VOUT', pin='5', side='right'),
                    ],
                    edgepadW = 1.5,
                    pinspacing=1.5).label('AP2127K-ADJTRG1', 'top', fontsize=12)
        d += AP2127
        d += (ic_gnd := elm.Ground()).at(AP2127.GND, dy=-d.unit * 1)
        d += elm.Wire().at(AP2127.GND).to(ic_gnd.start)

        d += (r_enable := elm.Resistor()).left().at(AP2127.EN).label('$R_{en}$').length(d.unit / 2)
        d += (bypass_in := elm.Capacitor()).down().at(AP2127.VIN, dx=-d.unit).label('$C_{in}$')
        d += elm.Dot().at(bypass_in.start)
        d += elm.Ground().at(bypass_in.end)
        d += (vin_wire := elm.Wire()).at(bypass_in.start).to(AP2127.VIN)
        d += elm.Line().left().at(bypass_in.start).length(d.unit / 4).label('$V_{in}$')
        d += elm.Wire('|-').at(AP2127.VIN, dx=-d.unit / 1.5).to(r_enable.end).idot()

        d += (r_top := elm.Resistor()).down().at(AP2127.VOUT, dx=d.unit / 2).label('$R_{top}$')
        d += (r_bot := elm.Resistor()).down().at(r_top.end).label('$R_{bottom}$')
        d += elm.Ground().at(r_bot.end)
        d += elm.Wire('|-').at(AP2127.ADJ).to(r_top.end)
        d += elm.Dot().at(r_top.end)
        d += elm.Wire().at(AP2127.VOUT).to(r_top.start)
        d += elm.Dot().at(r_top.start)
        d += (bypass_out := elm.Capacitor()).down().at(AP2127.VOUT, dx=d.unit).label('$C_{out}$')
        d += elm.Ground().at(bypass_out.end)
        d += elm.Wire().at(r_top.start).to(bypass_out.start)
        d += elm.Dot().at(bypass_out.start)
        d += elm.Line().right().at(bypass_out.start).length(d.unit / 4).label('$V_{out}$')

Microcontroller
---------------

:download:`ATSAMD21E18A <../datasheets/SAM-D21-DA1-Family-Data-Sheet-DS40001882H.pdf>` is a general
purpose ARM Cortex-M0 microcontroller.

Pinout
~~~~~~

.. flat-table:: Pin assignment
    :header-rows: 2

    * - :rspan:`1` Pin Number
      - :rspan:`1` Pin Name
      - :cspan:`2` Peripheral
      - :cspan:`2` Channel
      - :cspan:`2` Purpose

    * - MAX9621
      - MAX9921
      - AP22802
      - MAX9621
      - MAX9921
      - AP22802
      - MAX9621
      - MAX9921
      - AP22802

    * - 1
      - PA00
      - :cspan:`2` XTAL32
      - :cspan:`2` XIN32
      - :cspan:`2` External 32.768kHz oscillator input

    * - 2
      - PA01
      - :cspan:`2` XTAL32
      - :cspan:`2` XOUT32
      - :cspan:`2` External 32.768kHz oscillator output

    * - 3
      - PA02
      - ADC
      - PORT
      - EIC
      - AIN[0]
      - PORTA[02] (output)
      - EXTINT[2]
      - Channel 1 diagnostics
      - Output enable
      - Channel 1 overcurrent

    * - 4
      - PA03
      - ADC
      - :cspan:`1` EIC
      - AIN[1]
      - :cspan:`1` EXTINT[3]
      - Channel 2 diagnostics
      - Channel 1 input
      - Channel 2 overcurrent

    * - 5
      - PA04
      - :cspan:`2` EIC
      - :cspan:`2` EXTINT[4]
      - Channel 1 input
      - Error
      - Channel 1 input

    * - 6
      - PA05
      - :cspan:`2` EIC
      - :cspan:`2` EXTINT[5]
      - Channel 2 input
      - Channel 2 input
      - Channel 2 input

    * - 7
      - PA06
      - :cspan:`2` PORT
      - :cspan:`2` PORTA_06
      - Sleep (output inverted)
      - Diagnostics (output)
      - Channel 1 enable (output)

    * - 8
      - PA07
      - :cspan:`1`
      - PORT
      - :cspan:`1`
      - PORTA_07
      - :cspan:`1`
      - Channel 2 enable

    * - 11
      - PA08
      - :cspan:`2` UART
      - :cspan:`2` SERCOM PAD[0]
      - :cspan:`2` LIN Transmit (TX)

    * - 12
      - PA09
      - :cspan:`2` UART
      - :cspan:`2` SERCOM PAD[1]
      - :cspan:`2` LIN Receive (RX)

    * - 13
      - PA10
      - :cspan:`2` PORT
      - :cspan:`2` PORTA[10]
      - :cspan:`2` LIN Chip select

    * - 14
      - PA11
      - :cspan:`2` PORT
      - :cspan:`2` PORTA[11]
      - :cspan:`2` LIN Transmit enable/Fault detected

    * - 15
      - PA14
      - :cspan:`2` EIC
      - :cspan:`2` EXTINT[14]
      - :cspan:`2` IMU Accelerometer interrupt

    * - 16
      - PA15
      - :cspan:`2` EIC
      - :cspan:`2` EXTINT[15]
      - :cspan:`2` IMU Gyroscope interrupt

    * - 17
      - PA16
      - :cspan:`2` SERCOM1
      - :cspan:`2` SERCOM PAD[0]
      - :cspan:`2` IMU SPI Master In

    * - 18
      - PA17
      - :cspan:`2` SERCOM1
      - :cspan:`2` SERCOM PAD[1]
      - :cspan:`2` IMU SPI Clock

    * - 19
      - PA18
      - :cspan:`2` SERCOM1
      - :cspan:`2` SERCOM PAD[2]
      - :cspan:`2` IMU Accelerometer chip select

    * - 20
      - PA19
      - :cspan:`2` SERCOM1
      - :cspan:`2` SERCOM PAD[3]
      - :cspan:`2` IMU SPI Master out

    * - 21
      - PA22
      - :cspan:`2` PORT
      - :cspan:`2` PORTA[22]
      - :cspan:`2` IMU Gyroscope chip select

    * - 31
      - PA30
      - :cspan:`2` SWD
      - :cspan:`2` SWCLK
      - :cspan:`2` Serial-Wire-Debug clock

    * - 32
      - PA31
      - :cspan:`2` SWD
      - :cspan:`2` SWDIO
      - :cspan:`2` Serial-Wire-Debug data in/out

Schematic
~~~~~~~~~

See schematic notes in
:download:`ATSAMD21E18A <../datasheets/SAM-D21-DA1-Family-Data-Sheet-DS40001882H.pdf>`, section 45 -
page 1112.
