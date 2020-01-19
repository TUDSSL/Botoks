CHRT
====

The main idea is to combine multiple capacitive timekeepers of different sizes
into a Cascaded Hierarchical Remanence Timekeeper (CHRT).  The various tiers of
the CHRT can be used to minimize cold-boot time and energy consumption, and
maximize resolution and timekeeping range at run-time.  The tiers are linked
together, from smallest to largest, so that a depleted tier can automatically
activate the next tier, therefore increasing the total timing range, whilst
maintaining the best possible resolution.  For short time intervals, the smaller
tiers provide higher resolution and consume less energy.  The larger tiers are
used to time longer intervals, but have lower resolution and need more charging
energy.  To be able to use the CHRT, the cascaded tiers have to be pre-charged
at each reboot.  Our hardware abstraction layer, can be configured to minimize
energy consumption depending on the needs of an application and the expectations
of energy availability of the environment, by specifying how many tiers should
be pre-charged at each reboot.

.. image:: static/chrt.svg
   :width: 100 %
   :align: center

Programmers and intermittent kernel designers can choose to be either
conservative or adaptive with tier recharging.  Adaptive timekeeper provisioning
is useful when energy environments and application behavior are somewhat
predictable or continuous (for example, solar environments).  The basic idea of
adaptive tier recharge is to choose only the smallest tier that can still
satisfy the timing requirements.  The adaptive method saves energy since
overcharging the timekeeper when the kernel is only timing a short outage is
wasteful.  When the reboot frequency is variable the kernel may choose to be
conservative in timekeeper provisioning (tier recharging), as retrieved times
are more likely to be outside the timekeeping range of the current target tier.
For better robustness against variable reboot frequency, the user can request
the CHRT to charge more than just the current target tier.  Specifically, the
two function parameters KL and KR are used to tell the CHRT software layer to
charge all tiers in [Tx−KL,Tx+KR], given that Tx is the current target tier.
For instance, if KL = KR = 1, tiers Tx−1, Tx and Tx+1 would be recharged on
reboot, and the discharge would start from tier Tx−1, and continue with the
larger tiers in a cascaded fashion.  The parameters KL and KR control the
trade-off between timekeeping robustness and energy consumption, as charging
more tiers requires more energy.  In particular, KR has a higher impact on
energy consumption, due to larger tiers needing more charging energy.

.. _raw-api:

Raw API
-------

The raw CHRT interface is a hardware abstraction layer (HAL) of the underlying
timekeeping hardware functionality, to be used for low-level control of the
CHRT.  It is mostly intended as a building block for more advanced timekeeping
duties to be exposed by the runtime or kernel that has knowledge of the user
tasks and operations, but can be used at the application level as well by the
user.

Upon reboot first ``chrt_init()`` has to be called followed by
``chrt_get_time()`` to get the elapsed time of the power failure that was just
recovered from.  Next, the CHRT needs to be recharged using ``chrt_charge()``.

.. doxygengroup:: CHRT
   :project: botoks
   :content-only:

High-level API
--------------

The high-level CHRT interface enhances raw functionalities to provide
higher-level timekeeping tools to be used in real-world batteryless
applications for intermittent devices.  Fundamentally, this is implemented
combining CHRT functionalities with an on-board MCU digital timer to maintain
an always-available system time.  The system time is incremented at each reboot
using the raw ``chrt_get_time()`` function.  When queried during on-time, the
system time is combined with timing information retrieved from the digital
timer running in the background.  The system time is used to generate
timestamps and to set expiration timers for data and functions.

.. doxygengroup:: tptime
   :project: botoks
   :content-only:

Fine-tuning CHRT behavior
^^^^^^^^^^^^^^^^^^^^^^^^^

To tune the CHRT behavior when using the high-level API, you can tweak the
following parameters in ``tptime.h``.  Refer to ``chrt_init()`` in the
:ref:`raw API <raw-api>` for the meaning of each.

.. doxygengroup:: tptime-conf
   :project: botoks
   :content-only:

.. _chrt-calibration:

Calibration
-----------

Ideally, the RC circuit discharge model, t = −RC ln(V/V0), could be used to
estimate elapsed time, t.  In actuality, capacitance C and resistance R never
match their nominal values, and other parasitic capacitors and resistors are
spread through the circuit.  To resolve this issue, a software calibration
routine, to be performed before CHRT deployment, was devised and implemented,
aiming for a better precision and accuracy of the timekeeper.  During
calibration, all the tiers of the CHRT are repeatedly charged and discharged,
and their discharging profile is sampled over time to obtain a realistic
physical model for each tier.  This way, an interpolated version of the RC
circuit discharge model is built and stored in the form of a lookup table.  At
run-time, the voltage across the target tier is used to look up the table and
retrieve the corresponding elapsed time.

This process consists of two steps starting with the charge/discharge time
calibration, followed by accuracy calibration.

Charge/Discharge time calibration
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To calibrate the charging/discharging times please follow the process described below.

1. Connect Botoks to a debugger capable of debugging the MSP430 line of
   products.  *Set the target voltage to 2.2V* to avoid back-feeding.  Then
   power Botox using the auxiliary power connector on the PCB.  The voltage
   provided needs to be between 3.4 and 5V.

2. Program the calib-charge-discharge application using Uniflash.

   .. code:: bash

      $ ./flash.sh  bin/calib-charge-discharge.out

3. Connect to the programmers UART interface (Usually the higher of the two COM
   ports) with your favorite monitor software such as `picocom
   <https://linux.die.net/man/8/picocom>`_ and monitor the output.

   .. code:: bash

      $ picocom /dev/ttyACM1 -b 19200 --imap lfcrlf

4. Using the output modify the constants listed below in 'chrt.h'

To reach optimal accuracy the charge and discharge constants might need to
manually tuned.

.. doxygengroup:: CHRT_CHRCONF
   :project: botoks

.. doxygengroup:: CHRT_CHRCMP
   :project: botoks

.. doxygengroup:: CHRT_DISCONF
   :project: botoks



Accuracy calibration
^^^^^^^^^^^^^^^^^^^^

The accuracy calibration process is automated in the calibrate.py script.  This
scripts programs the accuracy calibration application, monitors the output,
creates a calibration file and then fits a RC curve using MATLAB on this
calibration data.  The process is repeated for each tier.  When the RC constants
for each tier is determined, lut.py is called to create a look-up table (lut.c)
based on the previously determined RC constants.  Upon completion of the program
the new lut.c can replace the provided lut.c.

From within the scripts folder:

.. code:: bash

   $ python calibrate.py
   $ mv -f lut.c ../libs/chrt/src/

To reach optimal accuracy the constants compensating for tier transitions might
need to manually tuned.  These constants are listed below.

.. doxygengroup:: CHRT_CASCONF
   :project: botoks

Examples
--------

Below some sample applications are listed to showcase the usage of the raw API and the
high-level API.

High-level API
^^^^^^^^^^^^^^^^^^^^
This example utilizes the high level API to check the if variable **foo_data**
has expired. Is simulates intermittency with a software reset and a delay.

.. literalinclude:: ../apps/high-level-interface/main.c
  :language: C

Raw API
^^^^^^^^^^^^^^^^^^^^

This example leverages the low level interface to measure time. Please note that
In this example the charging time of the CHRT is not compensated for.

.. literalinclude:: ../apps/raw-interface/main.c
  :language: C
