#include <efi/efi.h>

EFI_GUID DevicePathUtilitiesProtocolGuid = EFI_DEVICE_PATH_UTILITIES_PROTOCOL_GUID;
EFI_GUID DevicePathToTextProtocolGuid = EFI_DEVICE_PATH_TO_TEXT_PROTOCOL_GUID;
EFI_GUID DevicePathFromTextProtocolGuid = EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL_GUID;
EFI_GUID LoadedImageDevicePathProtocolGuid = EFI_LOADED_IMAGE_DEVICE_PATH_PROTOCOL_GUID;
EFI_GUID LoadedImageProtocolGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;

EFI_HANDLE ImageHandle;
EFI_SYSTEM_TABLE* SystemTable;
EFI_DEVICE_PATH_UTILITIES_PROTOCOL* DevicePathUtilitiesProtocol;
EFI_DEVICE_PATH_TO_TEXT_PROTOCOL* DevicePathToTextProtocol;
EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL* DevicePathFromTextProtocol;

void* malloc(UINTN size) {
    void* result;
    SystemTable->BootServices->AllocatePool(EfiConventionalMemory, size, &result);
    return result;
}

void free(void* ptr) {
    SystemTable->BootServices->FreePool(ptr);
}

UINTN strlen(const CHAR16* str) {
    UINTN len;
    for (len = 0; *str != u'\0'; str++, len++) ;
    return len;
}

void print(CHAR16* str) {
    SystemTable->ConOut->OutputString(SystemTable->ConOut, str);
}

void print_device_path(EFI_DEVICE_PATH_PROTOCOL* devicePath) {
    CHAR16* devicePathString;
    devicePathString = DevicePathToTextProtocol->ConvertDevicePathToText(devicePath, FALSE, TRUE);
    print(devicePathString);
    free(devicePathString);
}

EFI_INPUT_KEY wait_for_key() {
    EFI_INPUT_KEY key;
    while (SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &key) != EFI_SUCCESS) ;
    return key;
}

EFI_STATUS launch_app(CHAR16* kernel, CHAR16* command_line) {
    EFI_STATUS status;

    // init protocols
    status = SystemTable->BootServices->LocateProtocol(&DevicePathUtilitiesProtocolGuid, NULL, (void**)&DevicePathUtilitiesProtocol);
    if (status != EFI_SUCCESS) return status;

    status = SystemTable->BootServices->LocateProtocol(&DevicePathToTextProtocolGuid, NULL, (void**)&DevicePathToTextProtocol);
    if (status != EFI_SUCCESS) return status;

    status = SystemTable->BootServices->LocateProtocol(&DevicePathFromTextProtocolGuid, NULL, (void**)&DevicePathFromTextProtocol);
    if (status != EFI_SUCCESS) return status;

    // Get device path referring to current image
    EFI_DEVICE_PATH_PROTOCOL* currentDevicePath;
    status = SystemTable->BootServices->HandleProtocol(ImageHandle, &LoadedImageDevicePathProtocolGuid, (void**)&currentDevicePath);
    if (status != EFI_SUCCESS) return status;

    // Make device path referring to kernel
    CHAR16* devicePathString = DevicePathToTextProtocol->ConvertDevicePathToText(currentDevicePath, FALSE, TRUE);

    // find ) from end of string
    UINTN len = strlen(devicePathString);
    UINTN i;
    for (i = len; devicePathString[i] != u')'; i--) ;
    i++;

    // append path to kernel
    for (int j=0; kernel[j] != u'\0'; ) {
        devicePathString[i++] = kernel[j++];
    }
    devicePathString[i++] = u'\0';

    EFI_DEVICE_PATH_PROTOCOL* appDevicePath = DevicePathFromTextProtocol->ConvertTextToDevicePath(devicePathString);

    // print(u"Modified device path: ");
    // print_device_path(appDevicePath);
    // print(u"\r\n");

    // Load app image
    EFI_HANDLE appImageHandle;

    status = SystemTable->BootServices->LoadImage(TRUE, ImageHandle, appDevicePath, NULL, 0, &appImageHandle);
    if (status != EFI_SUCCESS) return status;

    EFI_LOADED_IMAGE_PROTOCOL* appImageProtocol;
    status = SystemTable->BootServices->HandleProtocol(appImageHandle, &LoadedImageProtocolGuid, (void**)&appImageProtocol);
    if (status != EFI_SUCCESS) return status;

    // Set linux command-line arguments
    appImageProtocol->LoadOptions = command_line;
    appImageProtocol->LoadOptionsSize = strlen(command_line) * sizeof(CHAR16);

    status = SystemTable->BootServices->StartImage(appImageHandle, NULL, NULL);

    SystemTable->BootServices->UnloadImage(appImageHandle);

    return status;
}

// EFI Image Entry Point
EFI_STATUS EFIAPI efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable) {

    // init globals
    ImageHandle = imageHandle;
    SystemTable = systemTable;

    // Clear screen
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

    // Launch app, returns on error, or if app exits
    CHAR16* kernel = u"/vmlinux";
    CHAR16* command_line = u"root=/dev/vda2 rw quiet";

    if (launch_app(kernel, command_line) != EFI_SUCCESS) {
        print(u"App failed to launch\r\n\r\n");
    }

    // Wait until keypress
    print(u"Press any key to shutdown...\r\n");
    wait_for_key();

    // Shutdown, does not return
    SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);

    // Should never get here
    return EFI_SUCCESS;
}
