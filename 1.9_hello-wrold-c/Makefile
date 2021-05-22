run:
	echo "Write EFI file to disk.img"
	hdiutil attach -mountpoint mnt disk.img
	mkdir -p mnt/EFI/BOOT
	ls -ld mnt/EFI/BOOT
	cp hello.efi mnt/EFI/BOOT/BOOTX64.efi
	ls -l mnt/EFI/BOOT/BOOTX64.efi

	sleep 1
	hdiutil detach mnt

	echo "Start QEMU"
	qemu-system-x86_64 \
		-drive if=pflash,format=raw,file=OVMF_CODE.fd \
		-drive if=pflash,format=raw,file=OVMF_VARS.fd \
		-drive file=disk.img,format=raw,index=0,media=disk

.PHONY: run

#  Step 1
image:
	echo "Create iamge"
	qemu-img create -f raw disk.img 200M
	mkfs.fat -n 'AOKI OS' -s 2 -f 2 -R 32 -F 32 disk.img

	echo "Download OVMF_CODE and OVMF_VARS"
	curl -O https://raw.githubusercontent.com/uchan-nos/mikanos-build/master/devenv/OVMF_CODE.fd
	curl -O https://raw.githubusercontent.com/uchan-nos/mikanos-build/master/devenv/OVMF_VARS.fd
	ls -l
.PHONY: iamge

#  Step 2
build:
	clang -target x86_64-pc-win32-coff -mno-red-zone -fno-stack-protector -fshort-wchar -Wall -c hello.c
	lld-link /subsystem:efi_application /entry:EfiMain /out:hello.efi hello.o
.PHONY: build
