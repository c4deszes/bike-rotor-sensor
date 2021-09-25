===================
Building block view
===================

Package view
------------

.. graphviz::
    digraph packages {
        node [shape=box];
        sensor-app -> bsp -> tiny20x-hal;
        sensor-app -> tiny20x-hal;
    }

.. note::
    The scope of this product is only the board support and application
    packages.

Block view
----------

.. graphviz::

    digraph boxed {
        graph [splines=ortho nodesep =.75];
        node [shape=box];
        edge [dir=both arrowtail=obox arrowhead=obox];
        { rank=same
            x1 -> x2 -> x3;
        }
        x1:s -> y1:n;
    }
