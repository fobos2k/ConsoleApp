#!/usr/bin/env bash

qemu-img create -f qcow2 win11.qcow2 64G
cp /usr/share/edk2-ovmf/x64/OVMF_VARS.4m.fd ./OVMF_VARS.fd
