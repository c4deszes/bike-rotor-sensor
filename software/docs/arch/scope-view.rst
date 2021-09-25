=================
Context and scope
=================

.. graphviz::

    graph Context {
        graph [rankdir = LR];

        node [shape=tab, style=filled];
        RotorSensor [label="Rotor sensor", fillcolor="coral"];
        Ecu [label="Electronic Control Unit"];
        Ecu1 [label="Electronic Control Unit 1"];
        Ecu2 [label="Electronic Control Unit 2"];

        node [shape=rectangle];
        CanBus [label="CAN Bus"];

        RotorSensor -- Ecu [label="Sensor data"];
        Ecu -- RotorSensor [label="Power"];
        Ecu -- CanBus;
        Ecu1 -- CanBus;
        Ecu2 -- CanBus;
    }

