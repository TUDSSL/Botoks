from subprocess import Popen, PIPE
from lut import createLUT
import subprocess
import argparse
import csv
import sys
import os

def programApplication(path):
    # check for connection
    #print("Starting debugger...")
    progProcess = Popen(["cd ../ &&" + "./flash.sh " + path],
     stdout=PIPE, stdin=PIPE, bufsize=1, universal_newlines=True, shell = True)

    # execute charge & discharge time calibration
    print("Programming in progress...")
    return progProcess

#------------------------------------------------------------------------------#
def retrieveOutput(currentFilename, port, baudRate):
    # To retrieve the output of the serial connection to the debugger.
    outputProcess = Popen(["exec " + "picocom" + " -b " + str(baudRate) + " --imap lfcrlf " + port], stdout=PIPE, stdin=PIPE, bufsize=1,
                            universal_newlines=True, shell = True)

    currentFile = None
    currentFileWriter = None

    while True:
        try:
            output = outputProcess.stdout.readline().replace('\n', '')
        except:
            print('Decoding error')
        #print(output)
        # found a new set of calibration data. Create/open a file.
        if 'Tier' in output:
            currentFile =  open(currentFilename, 'w')
            currentFileWriter = csv.writer(currentFile)

        # when we are done close the file
        if 'Done' in output:
            if(currentFile):
                currentFile.close()
            break
        # if a file is open and calib data is being received write the data to csv
        if currentFileWriter:
            # remove empty lines
            row = output.strip()
            if row:
                row = row.split(',')
                if len(row) == 4:
                    print("Calibrating tier " + row[3] + "% completed", end="\r")
                    currentFileWriter.writerow(row)
    print("\nAll data retrieved")
    return outputProcess



#------------------------------------------------------------------------------#

def computeFit(currentFilename):
    # execute accuracy calibration
    matlabProcess = Popen(["exec " + "matlab -nodisplay -nosplash -nodesktop -r " + " \"[rc, offset]=hrt_fit(\'"+ currentFilename + "\')\" "], stdout=PIPE, stdin=PIPE, bufsize=1,
                        universal_newlines=True, shell = True)
    while True:
        output = matlabProcess.stdout.readline()
        #print(output)
        if 'rc' in output:
            output = matlabProcess.stdout.readline()
            rc = float(matlabProcess.stdout.readline())
        if 'offset' in output:
            output = matlabProcess.stdout.readline()
            dc_offset = float(matlabProcess.stdout.readline())
            break

    matlabProcess.kill()
    return rc, dc_offset

#------------------------------------------------------------------------------#

parser = argparse.ArgumentParser(description='Botoks accuracy calibration routine')
parser.add_argument("-u", required=True, type=str, help="UART interface to the MSP430")
parser.add_argument("-b", required=False, type=int, default=19200, help="UART interface baudrate")
parser.add_argument("--force", required=False, type=bool, default=False, help="Overwrite existing calibration files")
args = parser.parse_args()

debugPort = args.u
baudRate = args.b
forceOverwrite = args.force

binaryPath = "bin/"

rc = []
offset = []
scale_per_tier = [1000, 100, 10, 1]

for i in range(4):
    binaryName = "calib-accuracy-t" + str(i) + ".out"
    currentFilename = 'calib-tier-'+ str(i) + ".csv"

    if not os.path.isfile(currentFilename) or forceOverwrite:
        print("Calibrating tier " + str(i))
        progProcess = programApplication(binaryPath + binaryName)
        outputProcess = retrieveOutput(currentFilename, debugPort, baudRate)
        progProcess.wait()
        print("Programming finished...")
        outputProcess.kill()
    else:
        print('Using existing calibration file, (--force to override).')


    tmp_rc, tmp_offset = computeFit(currentFilename)
    rc.append(tmp_rc / scale_per_tier[i])
    offset.append(tmp_offset)

    print("Tier " + str(i) + " computed RC: " + str(tmp_rc / scale_per_tier[i]) + ", Offset: " + str(tmp_offset))

print('Generating lookup table')
createLUT(rc, offset, scale_per_tier)

print('Done')
