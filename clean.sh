#!/bin/bash
set -e

pushd kpayload > /dev/null
make clean
popd > /dev/null

rm -rf tmp/*

pushd installer > /dev/null
make clean
rm -f source/*.inc.c
popd > /dev/null

rm -f hen.bin
