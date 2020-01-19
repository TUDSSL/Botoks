#!/bin/bash

# Docker image and tag.
IMAGE=cdelledonne/msp430-gcc
TAG=latest

# Mount point inside Docker container.
MOUNT_POINT=/Botoks

# UID and GID
PUID=`id -u $(whoami)`
PGID=`id -g $(whoami)`

# Target for 'make'.
TARGET=all

# Command-line options.
OPTS=`getopt -o t: --long target: -- "$@"`
while true; do
    case "$1" in
        -t | --target ) TARGET="$2"; shift 2 ;;
        -- ) shift; break ;;
        * ) break ;;
    esac
done

# Whether to build docs or not.
[[ $TARGET = "docs" ]] && BUILD_DOCS=1 || BUILD_DOCS=0

# Command for Docker container to execute.
COMMAND="\
mkdir -p build && cd build && \
cmake -DBUILD_DOCS=${BUILD_DOCS} .. && \
make ${TARGET}\
"

# Create Docker container and run build command.
docker run \
    --name=botoks \
    -e MSP430_TOOLCHAIN_PATH=/opt/ti/msp430-gcc \
    -u $PUID:$PGID \
    -v $PWD:${MOUNT_POINT} \
    -w ${MOUNT_POINT} \
    $IMAGE:$TAG \
    /bin/sh -c "${COMMAND}"

# Remove Docker container.
docker rm botoks
