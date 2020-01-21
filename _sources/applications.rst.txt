Applications
============

Botoks's code comes with simple applications intended to demonstrate its
usage and performance.

Bike speedometer
----------------

Botoks can be easily embedded on moving objects to extract some of their dynamic
behaviors.  For this application, we replaced Botoks's solar cell with a
magnetic energy harvester (one commonly found in inductive bike lights such as `SL150 Hub Lights
<https://www.reelight.com/products/hub-lights>`_), and attached it to the wheel
of a bike to measure its speed in RPM.  A magnet spins with the wheel and
charges the energy buffer on Botoks just enough that Botoks can turn on and
record the period of each revolution using its on-board CHRT and send it through
the radio.  A conceptual version of the application follows.

.. code-block:: c

   int main(void) {

     /* Get elapsed time using CHRT. */
     elapsed_time = get_elapsed_time();

     /* Add revolution period to radio packet. */
     packet.rev_period = elapsed_time;

     /* Send packet. */
     send_packet(&packet);
   }

Network synchronization
-----------------------

Botoks's timekeeping module, the CHRT, can be used for more complicated network
applications as well.  The :doc:`TPSync library <tpsync>` leverages the CHRT to
provide node-to-node network synchronization, and is used inside two simple
packet-exchange applications that run on Botoks.  One application, running on
one of the two nodes, is an intermittent radio transmitter, and the other,
running on the second node, is an intermittent radio receiver.  The transmitter
uses TPSync to compute its average wake-up (transmission) period, which sends
over the radio.  The receiver is configured to latch to that transmission period
as soon as it catches one packet from the transmitter.

Conceptually, the transmitter's code looks like the following

.. code-block:: c

   int main(void) {

     /* Get elapsed time using CHRT. */
     elapsed_time = get_elapsed_time();

     /* Compute average TX period and add it to packet. */
     avg_tx_period = get_avg_wakeup_period(elapsed_time);
     packet.avg_tx_period = avg_tx_period;

     /* Send packet. */
     send_packet(&packet);
   }

and the receiver's code like the following

.. code-block:: c

   int main(void) {

     /* Get elapsed time using CHRT. */
     elapsed_time = get_elapsed_time();

     /* Delay listening activity (avg_tx_period received in previous cycle). */
     delay_radio_listening(elapsed_time, packet.avg_tx_period, 0);

     /* Try to receive another packet. */
     receive_packet(&packet);
   }
