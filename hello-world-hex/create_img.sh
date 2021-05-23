#! /bin/bash

echo "Create iamge"
qemu-img create -f raw disk.img 200M
mkfs.fat -n 'AOKI OS' -s 2 -f 2 -R 32 -F 32 disk.img

echo "Download OVMF_CODE and OVMF_VARS"
curl -O https://raw.githubusercontent.com/uchan-nos/mikanos-build/master/devenv/OVMF_CODE.fd
curl -O https://raw.githubusercontent.com/uchan-nos/mikanos-build/master/devenv/OVMF_VARS.fd
ls -l
