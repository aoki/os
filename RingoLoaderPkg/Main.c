#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/LoadedImage.h>
#include <Uefi.h>

struct MemoryMap {
    UINTN buffer_size;
    VOID *buffer;
    UINTN map_size;
    UINTN map_key;
    UINTN descriptor_size;
    UINT32 descriptor_version;
};

// # Spec
// https://uefi.org/specifications#:~:text=UEFI%20Specification
// 244 pp @
// https://uefi.org/sites/default/files/resources/UEFI_Spec_2_9_2021_03_18.pdf
// ```
// EFI_STATUS
// (EFIAPI *EFI_GET_MEMORY_MAP) (
//    IN OUT UINTN *MemoryMapSize,
//    OUT EFI_MEMORY_DESCRIPTOR *MemoryMap,
//    OUT UINTN *MapKey,
//    OUT UINTN *DescriptorSize,
//    OUT UINT32 *DescriptorVersion
// );
// ```
EFI_STATUS GetMemoryMap(struct MemoryMap *map) {
    if (map->buffer == NULL) { return EFI_BUFFER_TOO_SMALL; }

    map->map_size = map->buffer_size;

    // 関数呼び出し時点のメモリマップを取得し、
    // 引数で指定した `MemoryMap`で指定された領域に書き込む。
    // 正常に取得できると `EFI＿SUCCESS` を返す
    return gBS->GetMemoryMap(
        // MemoryMapSize
        //  IN　メモリマップ書き込みようのメモリ領域の大きさ（バイト数）
        //  OUT 実際のメモリマップの大きさ
        &map->map_size,
        // MemoryMap
        //  IN メモリマップ書き込みようのメモリ領域の先頭ポインタ
        //  OUT INのポインタを元にメモリマップが書き込まれる
        (EFI_MEMORY_DESCRIPTOR *)map->buffer,
        // MapKey OUT メモリマップを識別するための値を書き込む変数
        &map->map_key,
        // DescriptorSize OUT
        // メモリマップのここの行を表すメモリディスクリプタのバイト数
        &map->descriptor_size,
        // DescriptorVersion OUT メモリディスクリプタの構造体のバージョン番号
        &map->descriptor_version);
}

EFI_STATUS OpenRootDir(EFI_HANDLE image_handle, EFI_FILE_PROTOCOL **root) {
    EFI_LOADED_IMAGE_PROTOCOL *loaded_image;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;

    gBS->OpenProtocol(image_handle, &gEfiLoadedImageProtocolGuid,
                      (VOID **)&loaded_image, image_handle, NULL,
                      EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    gBS->OpenProtocol(loaded_image->DeviceHandle,
                      &gEfiSimpleFileSystemProtocolGuid, (VOID **)&fs,
                      image_handle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    fs->OpenVolume(fs, root);

    return EFI_SUCCESS;
}

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE image_handle,
                           EFI_SYSTEM_TABLE *system_table) {
    Print(L"Hello, Ringo World!\n");

    CHAR8 memmap_buf[4096 * 4];
    struct MemoryMap memmap = {sizeof(memmap_buf), memmap_buf, 0, 0, 0, 0};
    GetMemoryMap(&memmap);

    EFI_FILE_PROTOCOL *root_dir;
    OpenRootDir(image_handle, &root_dir);

    EFI_FILE_PROTOCOL *memmap_file;
    root_dir->Open(
        root_dir, &memmap_file, L"\\memmap",
        EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);

    SaveMemoryMap(&memmap, memmap_file);
    memmap_file->Close(memmap_file);

    Print(L"All done\n");

    while (1) {};
    return EFI_SUCCESS;
}
