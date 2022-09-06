#!/bin/sh
./convert.sh Release
cd Release
sed -i 's/usr/avr-gcc-12.1.0-x64-linux/g' Makefile
make all
