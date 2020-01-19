import math
import logging, sys

### Configuration

# Constants
# ADC resolution
N = 12

# Limit to the amount of wasted power through discharging, in percent
H = 2
# Minimum seperation in adc steps
K = 10
# ADC reference voltage, also the voltage to which the capacitors are charged
V = 1.2
# The size of the discharge resistors in Ohm
R = 22e6

# Set of timing requirements
Requirements = [
[0.1,0.001]
, [1,0.01]
, [10,0.1]
, [100,1]
]

### end of configurable parameters.

# Selects from E12 capacitors
e12 = [1.0, 1.2, 1.5, 1.8, 2.2, 2.7, 3.3, 3.9, 4.7, 5.6, 6.8, 8.2]
# Currently supports capacitors from nF to uF
cScale = [1e-9, 1e-8, 1e-7, 1e-6]

# Variables
RC = 0
deltat = 1
CHRT = []
total_time = 0
sigma = 0

# Setup logging
logging.basicConfig(stream=sys.stderr, level=logging.INFO) # No debug info
# logging.basicConfig(stream=sys.stderr, level=logging.DEBUG) # Debug info

print("List of requirements:", Requirements)

def computeTier(req):
    deltat = req[1];
    Tmax = -math.inf
    Tmax_temp = -math.inf
    result = [deltat, Tmax, 0, 0]

    # Iterate over normal E12 capacitors within a certain specified scale.
    for cap_scale in cScale:
        for c in e12:
            # Compute current RC coefficient
            C = c * cap_scale
            RC = R * C

            # Compute T_max -> Tmax = RC * log((2^N) / K * (exp(deltat/RC) - 1))
            Tmax_temp = RC * math.log(math.pow(2,N) / K * (math.exp(deltat/RC) - 1))

            # Check the history one back to make sure we are not going down.
            # just to save some computations
            if Tmax_temp <= 0 or result[1] > Tmax_temp:
                return result

            # Compute the energy ratio (Limit the amount of unusable energy,
            # i.e. everything in the capacitor after T_max)
            energy_ratio = math.exp(-2*Tmax_temp/RC) * 100
            # Check we do not exceed the limit.
            if(energy_ratio > H):
                logging.debug("Stopping for energy ratio H\n")
                return result


            # Debug info
            #v_min = V * math.exp(-Tmax_temp/RC)
            #energy = 1/2 * C * math.pow(V,2)
            #wasted_energy = 1/2 * C * math.pow(v_min,2)
            #
            #print(deltat, Tmax_temp, "%.2e" % RC, "%.2e" % C, "%.2e" % energy,
            #    "%.2e" % wasted_energy, "Ratio:", "%.2f" % energy_ratio )

            # Verify that Tmax_temp is increasing.
            if(Tmax_temp > 0 and Tmax_temp > Tmax):
                Tmax = Tmax_temp
                result = [deltat, Tmax, RC, C]
                #print(RC)
                if Tmax >= req[0]:
                    # We want to stop as early as possible to avoid
                    # wasting energy on extended timing ranges
                    # that are not strictly required
                    logging.debug("Stopping requirement satisfied\n")
                    return result
    logging.error("Error results invalid\n")
    return result



for req in Requirements:
    if(req[0] > sigma):
        current_tier = computeTier(req)

        number_of_duplicates = math.ceil(req[0]/current_tier[1])
        total_time += current_tier[1] * number_of_duplicates
        current_tier.append(number_of_duplicates)

        CHRT.append(current_tier)
        sigma += current_tier[1]

print("\nResults:")
print("CHRT Configuration: \n(Tier) Resolution (s), Time-able range (s), RC coefficient, Capacitance (F), Number of repeated tiers ", *CHRT, sep = "\n")
print("(CHRT) Total time-able range (Excluding leakage in seconds):", total_time)
