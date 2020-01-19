#!/bin/bash

UNIFLASH_PATH=${UNIFLASH_PATH:-/opt/ti/uniflash}

if [ $# != 1 ]; then
    echo "Usage: ./flash.sh bin/<app-name>.out"
    exit
fi

VERSION=($(cat ${UNIFLASH_PATH}/uniflash/public/version.txt | tr "." "\n"))
MAJOR=${VERSION[0]}

CONFIG_FILE=config/uniflash-v${MAJOR}-MSP430FR5994.ccxml

${UNIFLASH_PATH}/dslite.sh \
    -c ${CONFIG_FILE} \
    -l config/botoks.ufsettings \
    -b Erase \
    -f $1
