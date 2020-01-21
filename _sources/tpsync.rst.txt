TPSync
======

TPSync is a rudimentary node-to-node synchronization library for MSP430-based
transiently-powered wireless sensors.  It provides a simple API to synchronize
radio schedules of a transmitter and a receiver based on timing information
collected with an intermittency-safe timekeeper (e.g., the :doc:`chrt`).

Radio synchronization
---------------------

TPSync allows a receiving node to synchronize to a periodic transmission
schedule of a transmitting node.  For this to work, the transmitter attaches the
current transmission period, i.e., the time interval between two packets, to
every sent packet.

If the transmission period is somewhat stable, the receiver can try to latch to
that (or a multiple of that, in case the energy is not enough), and turn on its
radio only when a packet is being transmitted.  As soon as the receiver
successfully receives a packet, it uses the transmission period contained in the
packet to calculate how long to delay its next reception attempt.  For instance,
in the figure below, the receiver latches to the transmitter's schedule after
successfully receiving the third packet.

.. image:: static/sync-1.svg
   :width: 80 %
   :align: center

As shown below, the transmitter could compute its average wake-up period
(transmission period) and send it using the radio

.. code-block:: c

   /* Get elapsed time using CHRT. */
   elapsed_time = get_elapsed_time();

   /* Compute average TX period and add it to packet. */
   avg_tx_period = get_avg_wakeup_period(elapsed_time);
   packet.avg_tx_period = avg_tx_period;

   /* Send packet. */
   send_packet(&packet);

and the receiver could use the transmission period to delay its next listening
activity

.. code-block:: c

   /* Get elapsed time using CHRT. */
   elapsed_time = get_elapsed_time();

   /* Delay listening activity (avg_tx_period received in previous cycle). */
   delay_radio_listening(elapsed_time, packet.avg_tx_period, 0);

   /* Try to receive another packet. */
   receive_packet(&packet);

The methods used in the examples above are documented here.

.. doxygengroup:: tpsync
   :project: botoks
   :content-only:

Synchronization error
---------------------

When the receiver starts listening too early, some radio time is wasted (idle
listening), potentially leading to premature power loss and packet miss.  To
correct for such a synchronization error, the receiver can track idle time and
try to minimize it in the next cycle.

To do so, it is sufficient for the receiver to call ``track_sync_error_start()``
just before turning on its radio, and  ``track_sync_error_stop()`` when
successfully receiving a packet.  The receiver's code can be extended as
follows:

.. code-block:: c

   /* Get elapsed time using CHRT. */
   elapsed_time = get_elapsed_time();

   /* Delay listening activity (avg_tx_period received in previous cycle). */
   delay_radio_listening(elapsed_time, packet.avg_tx_period, 0);

   /* Start tracking sync error. */
   track_sync_error_start();

   /* Try to receive another packet. */
   receive_packet(&packet);

   /* Stop tracking sync error. */
   track_sync_error_stop();

TPSync will internally calculate the receiver's idle time, after receiving a
packer, and try to minimize it in the next ``delay_radio_listening()``.

.. doxygengroup:: tpsync-sync-err
   :project: botoks
   :content-only:

Fine-tuning TPSync
------------------

.. doxygengroup:: tpsync-conf
   :project: botoks
   :content-only:
