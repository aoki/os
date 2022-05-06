bold := $(shell tput bold)
reset := $(shell tput sgr0)
EDK2 := ${HOME}/work/src/github.com/tianocore/edk2
LOADER_NAME := RingoLoader
LOADER_DIR := $(LOADER_NAME)Pkg

clean:
	rm $(EDK2)/$(LOADER_DIR)
	# cd $(EDK2); \
	# 	make -C ${EDK2}/BaseTools/Source/C clean;
	rm $(EDK2)/Conf/target.txt
	rm ${EDK2}/Build/$(LOADER_NAME)X64/DEBUG_CLANGPDB/X64/$(LOADER_DIR)/Loader/DEBUG/Loader.efi
	rm Loader.efi
	rm disk.img
.PHONY: clean

# Step 1
build:
	@printf '\n$(bold)# Cechkout edk2$(reset)\n'
	@cd $(EDK2); \
		git co edk2-stable202102;\
		git submodule update

	@printf '\n$(bold)# Create a symbolic link to the EDK2 directory from the $(LOADER_DIR)$(reset)\n'
	ln -s ${PWD}/$(LOADER_DIR) $(EDK2) || :


	# @printf '\n$(bold)# Make base BaseTools$(reset)\n'
	# @cd $(EDK2); \
	# 	make -C ${EDK2}/BaseTools/Source/C

	@printf '\n$(bold)# Copy target.txt and build$(reset)\n'
	@cd $(EDK2); \
		source edksetup.sh;\
		rm $(EDK2)/Conf/target.txt;\
		ln -s ${PWD}/$(LOADER_DIR)/target.txt $(EDK2)/Conf/target.txt;\
		cat ${EDK2}/Conf/target.txt;\
		build;\
		ls -l ${EDK2}/Build/$(LOADER_NAME)X64/DEBUG_CLANGPDB/X64/$(LOADER_DIR)/Loader/DEBUG/Loader.efi

	@printf '\n$(bold)# Copy EFI file$(reset)\n'
	cp ${EDK2}/Build/$(LOADER_NAME)X64/DEBUG_CLANGPDB/X64/$(LOADER_DIR)/Loader/DEBUG/Loader.efi .
.PHONY: build

# Step 2
image:
	@printf '\n$(bold)# Create image$(reset)\n'
	qemu-img create -f raw disk.img 200M
	mkfs.fat -n 'AOKI OS' -s 2 -f 2 -R 32 -F 32 disk.img

	@printf '\n$(bold)# Download OVMF_CODE and OVMF_VARS$(reset)\n'
	curl -O https://raw.githubusercontent.com/uchan-nos/mikanos-build/master/devenv/OVMF_CODE.fd
	curl -O https://raw.githubusercontent.com/uchan-nos/mikanos-build/master/devenv/OVMF_VARS.fd
	ls -ltr
.PHONY: iamge

# Step 3
run:
	@printf '\n$(bold)# Write a EFI file to the disk.img$(reset)\n'
	hdiutil attach -mountpoint mnt disk.img
	mkdir -p mnt/EFI/BOOT
	ls -ld mnt/EFI/BOOT
	cp Loader.efi mnt/EFI/BOOT/BOOTX64.efi
	ls -l mnt/EFI/BOOT/BOOTX64.efi

	sleep 1
	hdiutil detach mnt

	@printf '\n$(bold)# Start QEMU$(reset)\n'
	qemu-system-x86_64 \
		-drive if=pflash,format=raw,file=OVMF_CODE.fd \
		-drive if=pflash,format=raw,file=OVMF_VARS.fd \
		-drive file=disk.img,format=raw,index=0,media=disk
.PHONY: run

mount:
	hdiutil attach -mountpoint mnt disk.img
.PHONY: mount

umount:
	hdiutil detach mnt
.PHONY: umount
