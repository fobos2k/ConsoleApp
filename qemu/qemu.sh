#!/bin/bash

ISO="Win11_24H2_English_x64.iso"
VIRTIO_ISO="virtio-win.iso"
DISK="win11.qcow2"
TPM_SOCKET="/tmp/win11_tpm.sock"

# Убедимся, что нет старого TPM
rm -f "$TPM_SOCKET"
mkdir -p tpmstate

# Старт TPM
swtpm socket \
  --tpm2 \
  --tpmstate dir=./tpmstate \
  --ctrl type=unixio,path=$TPM_SOCKET \
  --log level=20 \
  --daemon

# Ждём появления сокета
for i in {1..10}; do
  if [ -S "$TPM_SOCKET" ]; then break; fi
  sleep 0.2
done

# Запуск QEMU
qemu-system-x86_64 \
  -enable-kvm \
  -m 8G \
  -cpu host \
  -smp 4 \
  -machine q35,smm=on \
  -device intel-iommu \
  -device usb-host,vendorid=0x1050,productid=0x0407 \
  -drive if=pflash,format=raw,readonly=on,file=/usr/share/edk2-ovmf/x64/OVMF_CODE.secboot.4m.fd \
  -drive if=pflash,format=raw,file=./OVMF_VARS.fd \
  -drive file=$DISK,if=virtio,format=qcow2 \
  -drive file=$VIRTIO_ISO,index=3,media=cdrom \
  -cdrom "$ISO" \
  -boot order=d \
  -chardev socket,id=chrtpm,path=$TPM_SOCKET \
  -tpmdev emulator,id=tpm0,chardev=chrtpm \
  -device tpm-tis,tpmdev=tpm0 \
  -netdev user,id=net0,hostfwd=tcp::2222-:22 \
  -device e1000,netdev=net0 \
  -vga virtio \
  -usb -device usb-tablet
