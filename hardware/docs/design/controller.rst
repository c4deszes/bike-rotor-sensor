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

.. list-table:: Pinout
    :header-rows: 1

    * - Pin Number
      - Pin Name
      - Peripheral
      - Channel
      - Purpose

    * - 1
      - PA00
      - XTAL32
      - XIN32
      - External 32.768kHz oscillator input

    * - 2
      - PA01
      - XTAL32
      - XOUT32
      - External 32.768kHz oscillator output

    * - 3
      - PA02
      - 
      - 
      - Channel 1 diagnostics (MAX9621), Output enable (MAX9921), Channel 1 overcurrent (AP22802)

    * - 4
      - PA03
      - 
      - 
      - Channel 2 diagnostics (MAX9621), Channel 1 input (MAX9921), Channel 2 overcurrent (AP22802)

    * - 5
      - PA04
      - EIC
      - EXTINT[4]
      - Channel 1 input (MAX9621), Error (MAX9921), Channel 1 input (AP22802)

    * - 6
      - PA05
      - EIC
      - EXTINT[5]
      - Channel 2 input (MAX9621), Channel 2 input (MAX9921), Channel 2 input (AP22802)

    * - 7
      - PA06
      - 
      - 
      - Output stage enable (MAX9621), Output stage diagnostics (MAX9921), Channel 1 enable (AP22802)

    * - 8
      - PA07
      - PORT
      - PORTA_07
      - Channel 2 enable (AP22802)

    * - 11
      - PA08
      - SERCOM0
      - PAD[0]
      - LIN Transceiver transmit

    * - 12
      - PA09
      - SERCOM0
      - PAD[1]
      - LIN Transceiver receive

    * - 13
      - PA10
      - PORT
      - PORTA_10
      - Voltage regulator enable

    * - 15
      - PA14
      - EIC
      - EXTINT[14]
      - Accelerometer interrupt

    * - 16
      - PA15
      - EIC
      - EXTINT[15]
      - Gyroscope interrupt

    * - 17
      - PA16
      - SERCOM1
      - PAD[0]
      - SPI Master In

    * - 18
      - PA17
      - SERCOM1
      - PAD[1]
      - SPI Clock

    * - 19
      - PA18
      - SERCOM1
      - PAD[2]
      - Accelerometer chip select

    * - 20
      - PA19
      - SERCOM1
      - PAD[3]
      - SPI Master out

    * - 21
      - PA22
      - PORT
      - PORTA_22
      - Gyroscope chip select

    * - 31
      - PA30
      - SWD
      - SWCLK
      - Serial-Wire-Debug clock

    * - 32
      - PA31
      - SWD
      - SWDIO
      - Serial-Wire-Debug data in/out

Schematic
~~~~~~~~~


