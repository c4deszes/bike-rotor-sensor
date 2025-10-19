Rotor Sensor System
===================

.. toctree::
    :maxdepth: 4
    :hidden:

    reqs/index
    fmea/index
    arch/index
    design/index
    tests/index

The rotor sensor system is responsible for measuring the bike's speed, orienation and altitude, or
really anything that has to do with the bike's motion.

It's name originates from the initial concept in which precise knowledge about the wheels'
position would have been maintained.

Product generations
-------------------

Gen1.0
~~~~~~

.. container:: gen1

    .. container:: leftside

        .. figure:: _static/resources/gen1-mounted.png
            :scale: 50%

    .. container:: rightside

        The first generation used a concept where the sensor would only measure one wheel.
        Mechanical design was only completed for the front wheel, the device mounted on the fork and
        used the disc brake rotor as the reference.

        Some notable issues:

        * Mounting is not reliable
        * Device is overall very large
        * Optical sensor wasn't reliable, sun light affected its output

        Because of these issues the next generations had more responsibilities and had to have smaller
        footprints.

Gen2.0
~~~~~~

.. container:: gen2

    .. container:: leftside

        .. figure:: _static/resources/gen2-mounted.png
            :scale: 50%

    .. container:: rightside

        The second generation measures both wheel speeds. To reduce the wiring and increase sensor
        reliability it uses 2-wire magnetic sensors similar to what would be used in cars. It also
        supports 3-wire sensors through a different hardware BOM and case.

        Some issues:

        * Debug connector is quite large
        * PCB size is increased due to 3-wire sensor variant support
        * Case is somewhat large due to screws and threaded inserts
        * Wasted power because of linear regulators

        Design overall works well.

Gen2.1
~~~~~~

.. container:: gen2.1

    .. container:: leftside

        .. figure:: _static/resources/gen2.1-mounted.png
            :scale: 50%

    .. container:: rightside

        This is a revision of Gen2.0 and it improves on lots of aspects of the design. Primarily the
        footprint is reduced while functionality is increased.

        This version only uses 2-wire sensors and it adds one extra for measuring cadence. Additional
        sensing capabilities include a pressure sensor which is used to estimate altitude and
        calculate elevation changes.

        The bus connector is replaced with a 3pin variant and the case is glued instead of using screws.

        Issues as of now:

        * Pressure sensor doesn't seem to be accurate
        * Capacitor on the LIN transceiver chip select line is missing, bootloader entry doesn't work
        * MAX9921 cannot report sensor failures individually without running diagnosis
        * Software time measurement isn't accurate 21 seconds lost over 1 hour
