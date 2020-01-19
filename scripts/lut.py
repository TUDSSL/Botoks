import math

def createLUT(rc_per_range, comp_per_range, scale_per_tier):
    if (len(rc_per_range) != len(comp_per_range) != len(scale_per_tier)):
        return

    N_TIERS = len(rc_per_range)

    log_lut = [[0 for x in range(4096)] for y in range(N_TIERS)]

    for j in range(N_TIERS):
        for i in range(1,4096):
            if((i - comp_per_range[j]) > 0):
                log_lut[j][i] = min(int(
                round(-math.log((i - comp_per_range[j])/ 4095.0) * rc_per_range[j] * scale_per_tier[j])
                ), 65535)
            else:
                log_lut[j][i] = 65535
    with open("lut.c", "w") as f:
        f.write("#include <chrt.h>\n")
        f.write("#include <mspbase.h>\n")
        f.write("const uint16_t log_lut[N_TIERS][TABLE_LEN] = {\n")
        for j in range(N_TIERS):
            f.write("    {")
            for i in range(4095):
                f.write("{}, ".format(log_lut[j][i]))
            f.write("{}".format(log_lut[j][4095]))
            f.write("},\n")

        f.write("};")
