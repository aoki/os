#! /bin/bash

echo "Write EFI file to disk.img"
hdiutil attach -mountpoint mnt disk.img
mkdir -p mnt/EFI/BOOT
ls -ld mnt/EFI/BOOT
cp BOOTX64.EFI mnt/EFI/BOOT/BOOTX64.EFI
ls -l mnt/EFI/BOOT/BOOTX64.EFI

sleep 1
hdiutil detach mnt

echo "Start QEMU"
qemu-system-x86_64 \
  -drive if=pflash,format=raw,file=OVMF_CODE.fd \
  -drive if=pflash,format=raw,file=OVMF_VARS.fd \
  -drive file=disk.img,format=raw,index=0,media=disk
