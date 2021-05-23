#include <Library/UefiLib.h>
#include <Uefi.h>

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE image_handle,
                           EFI_SYSTEM_TABLE *system_table) {
    Print(L"Hello, Ringo World!\n");
    while (1) {};
    return EFI_SUCCESS;
}
