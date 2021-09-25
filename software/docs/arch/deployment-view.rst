===============
Deployment view
===============

.. graphviz::

    graph diagram {
        compound=true;
        ranksep=1

        subgraph cluster_all {
            label="System"

            subgraph cluster_motion_controller {
                label="Motion Controller"
                node [shape=component, style=filled];
                RotorSensor [label="Rotor sensor", fillcolor="coral"];
                Ecu [label="Electronic Control Unit"];
                Ecu -- RotorSensor;
            }
        }
    }

