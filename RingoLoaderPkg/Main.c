#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
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

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE image_handle,
                           EFI_SYSTEM_TABLE *system_table) {
    Print(L"Hello, Ringo World!\n");
    while (1) {};
    return EFI_SUCCESS;
}
