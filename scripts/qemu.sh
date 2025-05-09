#!/usr/bin/env bash

QEMU_DIR="./qemu"
TPM_STATE_DIR="${QEMU_DIR}/tpmstate"

LIST_PACKAGES="wget qemu virt-manager edk2-ovmf swtpm"

VIRTIO_ISO="${QEMU_DIR}/virtio-win.iso"
VIRTIO_URL="https://fedorapeople.org/groups/virt/virtio-win/direct-downloads/stable-virtio/$(basename ${VIRTIO_ISO})"

WINDOWS_ISO="${QEMU_DIR}/Win11_24H2_English_x64.iso"
WINDOWS_URL="https://www.microsoft.com/en-us/software-download/windows11"

VM_DISK="${QEMU_DIR}/win11-vm.qcow2"
OVMF_VARS="${QEMU_DIR}/OVMF_VARS.fd"
TPM_SOCKET="/tmp/win11_tpm.sock"

_download_iso()
{
    [ -z $1 ] && exit 1
    wget --continue --directory-prefix=${QEMU_DIR} $1
}

_check_env()
{
    mkdir -p "${QEMU_DIR}"

    if [ ! -e ${WINDOWS_ISO} ]; then
        echo -e "\e[1;31mERROR! \e[0;31mWindows ISO isn't found!\nPlease download from ${WINDOWS_URL}\e[0m"
        exit 1
    fi

    if [ ! -e ${VIRTIO_ISO} ]; then
        echo -e "\e[1;33mWARNING! \e[0mVirtio ISO isn't found..."
        _download_iso ${VIRTIO_URL}
    fi

    which paru &> /dev/null
    if [ $? -ne 0 ]; then
        echo -e "\e[1;31mERROR! \e[0;31mparu isn't found...\e[0m"
        exit 1
    fi

    paru -Sy --needed ${LIST_PACKAGES}
}

_activate_tpm()
{
    if [ -S "$TPM_SOCKET" ]; then
        if ! lsof "$TPM_SOCKET" &>/dev/null; then
            echo "Stale TPM socket found — removing..."
            rm -f "$TPM_SOCKET"
        else
            echo "TPM socket already active — skipping swtpm startup"
        fi
    fi

    if [ ! -S "$TPM_SOCKET" ]; then
        echo "Starting swtpm..."
        mkdir -p "$TPM_STATE_DIR"
        swtpm socket \
              --tpm2 \
              --tpmstate dir="$TPM_STATE_DIR" \
              --ctrl type=unixio,path="$TPM_SOCKET" \
              --log level=20 \
              --daemon
    fi

    for i in {1..10}; do
        if [ -S "$TPM_SOCKET" ]; then break; fi
        sleep 0.2
    done
}

create()
{
    _check_env
    
    qemu-img create -f qcow2 ${VM_DISK} 64G
    cp /usr/share/edk2-ovmf/x64/OVMF_VARS.4m.fd ${OVMF_VARS}
}

run()
{
    _activate_tpm

    echo "Starting QEMU VM..."
    qemu-system-x86_64 \
        -enable-kvm \
        -m 8G \
        -cpu host \
        -smp 4 \
        -machine q35,smm=on \
        -device intel-iommu \
        -device usb-host,vendorid=0x1050,productid=0x0407 \
        -drive if=pflash,format=raw,readonly=on,file=/usr/share/edk2-ovmf/x64/OVMF_CODE.secboot.4m.fd \
        -drive if=pflash,format=raw,file=${OVMF_VARS} \
        -drive file=${VM_DISK},if=virtio,format=qcow2 \
        -drive file=${VIRTIO_ISO},index=3,media=cdrom \
        -cdrom "${WINDOWS_ISO}" \
        -boot order=d \
        -chardev socket,id=chrtpm,path=${TPM_SOCKET} \
        -tpmdev emulator,id=tpm0,chardev=chrtpm \
        -device tpm-tis,tpmdev=tpm0 \
        -netdev user,id=net0,hostfwd=tcp::2222-:22 \
        -device e1000,netdev=net0 \
        -vga virtio \
        -usb -device usb-tablet \
        &
}

[ ! -z $1 ] && $1
