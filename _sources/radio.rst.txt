Radio
==========

To interface with the radio present on the Botoks platform (`ZL70550 <https://www.microsemi.com/product-directory/sub-ghz-radio-transceivers/3928-zl70550>`__) we developed a
rudimentary driver. This library is capable of executing calibration,
transmitting and receiving packets.

First the driver needs to be initialized using ``zl70550_init()`` next the
common registers have to be restored using ``zl70550_restore_state()`` and
the operating frequency has to be set ``zl70550_set_frequency()``.

With the registers in the correct state now calibration can be executed
``zl70550_calibrate()``.
Calibration is only executed once on the first boot, afterwards it applies the
previously determined calibration. This calibration is stored in FRAM.

After calibration the radio is fully operational and can transmit
``zl70550_packet_transmit()``  or receive ``zl70550_packet_receive()``.

.. doxygengroup:: ZL70550
   :project: botoks
   :content-only:
