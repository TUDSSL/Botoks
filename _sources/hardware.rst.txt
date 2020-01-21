Hardware
==========

        .. list-table::

            * - .. figure:: static/botoks-top.png
              - .. figure:: static/botoks-bottom.png

For the full hardware schematics, please refer to :download:`Schematics <../hardware/schematic/Botox_Mote.pdf>`.

CHRT Design
-----------

On Botoks, the CHRT is implemented with off the shelf SMD components. To support
a wide variety of applications the following requirements are set for the CHRT.
These requirements are listed in [time-able range, resolution].

.. code-block:: python

   Requirements = [
     [0.1,0.001]
     , [1,0.01]
     , [10,0.1]
     , [100,1]
     ]

By entering these requirements in ``chrt.py`` found in the scripts folder and executing the script, these requirements are translated into a CHRT configuration.

.. code-block:: bash

   $ python chrt.py

Resulting in a configuration of 4 tiers with 2.2nF, 22nF, 220nF and 2.2uF capacitors,
as present on the Botoks sensor.

The scripts operates by iterating over different capacitor sizes to find a suggested
CHRT configuration for the requirements.
