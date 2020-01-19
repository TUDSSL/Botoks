# Hierarchical Remanence Timekeeper (HRT) Library for MSP430

This library contains methods for:
- calibrating a HRT circuit
- recharging a timekeeper
- querying a timekeeper for elapsed time

## Methods

Methods documentation is provided in [`include/timekeeper.h`][header].
The available methods are recapped here.

- Calibrate timekeeper circuit (diode + capacitor + resistor) for timing
intervals at a minimum granularity of 96 us:

        uint8_t timekeeper_calibrate(uint8_t tk_id, uint16_t start, uint16_t end, uint16_t step)


- Charge timekeeper to a fixed voltage:

        uint8_t timekeeper_charge(uint8_t tk_id)

- Get time since last recharge of the timekeeper using the ADC:

        uint16_t timekeeper_get_time(uint8_t tk_id)

## Usage

### Calibration

Before the HRT can be used, it has to be calibrated **once** (only upon first boot).
Every tier needs calibration for the range of time intervals it is meant to capture.
For instance, if the timekeeper with ID `0` is to be used to capture time
intervals from 10 to 50 ms, with a granularity of 1000 us, and the timekeeper with ID `1`
is to be used to capture intervals from 50 to 200 ms, with the same granularity,
an application using this 2-tier HRT can be like

```c
NV uint8_t first_boot = 1; // non-volatile flag

void main()
{
    init(); // perform needed initializations

    if (first_boot) {
        timekeeper_calibrate(0, 10, 50, 1000);
        timekeeper_calibrate(1, 50, 200, 1000);
        first_boot = 0;
    }

    routine(); // all the rest
}
```

Currently, only 3 tiers are supported, so the ID can be `0`, `1` or `2`.
Interval bounds `start` and `end` must be specified in terms of milliseconds,
and granularity `step` in terms of microseconds. The step can be as low as 96 us.

The outcome of the calibration is "printed" over eUSCI_A0 (_backchannel_ UART on
the launchpad), configured with a Baudrate of 115200.
Each line of the print represents an interval of ADC values associated to one
time interval, e.g.

```
300 : 400 => 729 (70 ms)
```

means that all the ADC values between 300 and 400 will correspond to a timer value
of 729 ticks, equal to 70 ms.

### Timekeeping

Upon each reboot, the HRT has to be queried, to retrieve elapsed time, and then
one of the tiers has to be recharged.
Suppose you want to use a 2-tier HRT, a time boundary between the two tiers must
be chosen in order to select which tier to use when.
For a boundary placement heuristic, consult my [MSc thesis][thesis] (section X.X).

As an example, consider a 2-tier HRT, where the smaller capacitor is used up to
45 ms, and the bigger from 45 ms onward.
At the beginning of your main routine, you have to query the HRT and then issue
the recharge command

```C
NV uint8_t first_boot = 1; // non-volatile flag
uint16_t time_boundary[N_TIERS - 1] = {45};

void main()
{
    init();

    if (first_boot) {
        timekeeper_calibrate(0, 10, 50, 1000);
        timekeeper_calibrate(1, 50, 200, 1000);
        first_boot = 0;
    }

    uint16_t elapsed_time = timekeeper_get_time();
    uint32_t time_us = elapsed_time * US_PER_TICK; // to convert ticks into us
    timekeeper_charge(N_TIERS, time_boundary);

    routine(); // all the rest
}
```

and the library will recharge the correct tier.

### Tuning Accuracy and Overhead

Calibration and timekeeping accuracy depends on some factors.
Some parameters can be tuned in [`include/timekeeper.h`][header].

The ADC has a resolution of 12 bits.
Each ADC value can be halved `ADC_SCALING` times, resulting in
2<sup>(12 >> `ADC_SCALING`)</sup> possible values.
`ADC_SCALING` has a default value of `0`, but the user can increase this
parameter to reduce the amount of memory needed for the time lookup table.

Moreover, during calibration, each timer interval is calibrated `SAMPLES_PER_TEST`
times, and then the average result is computed.
To reduce calibration run time, this parameter can be decreased, with the risk
of a lower calibration accuracy.

Finally, during calibration and during recharge, the two parameters
`DISCHARGE_TIME` and `CHARGE_TIME` control the amount of microseconds allowed
for discharging and recharging the HRT.
Smaller values yield lower energy consumption, but worse consistency.

### Other Settings

The number of tiers can be set using `N_TIERS` (currently up to 3 tiers supported).

Charge and discharge GPIO pins can be selected using `CHARGE_PORT`, `DISCHARGE_PORT`,
`CHARGE_PIN_x` and `DISCHARGE_PIN_x`.

### Example

A full example application calibrating and testing a HRT can be found [here][example-app].

## More Info

- You are advised to read my [MSc thesis][thesis] before experimenting with this library,
to get a better understanding of the Hierarchical Remanence Timekeeper.

- In addition, the documentation provided in [`include/timekeeper.h`][header] will guide you through the usage of the library.

- In case of further doubts, [check the issues][issues] (or fill in a new one).

[header]: https://github.com/cdelledonne/intermittent-network-stack/blob/master/libs/timekeeper/include/timekeeper.h
[example-app]: https://github.com/cdelledonne/intermittent-network-stack/tree/master/apps/tk-test
[thesis]: insert-link
[issues]: https://github.com/cdelledonne/intermittent-network-stack/issues
