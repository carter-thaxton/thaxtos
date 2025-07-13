#include <efi/efi.h>

EFI_GUID DevicePathUtilitiesProtocolGuid = EFI_DEVICE_PATH_UTILITIES_PROTOCOL_GUID;
EFI_GUID DevicePathToTextProtocolGuid = EFI_DEVICE_PATH_TO_TEXT_PROTOCOL_GUID;
EFI_GUID DevicePathFromTextProtocolGuid = EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL_GUID;
EFI_GUID LoadedImageDevicePathProtocolGuid = EFI_LOADED_IMAGE_DEVICE_PATH_PROTOCOL_GUID;


EFI_STATUS launch_app(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    EFI_STATUS status;

    // Access device path protocols
    EFI_DEVICE_PATH_UTILITIES_PROTOCOL* DevicePathUtilitiesProtocol;
    status = SystemTable->BootServices->LocateProtocol(&DevicePathUtilitiesProtocolGuid, NULL, (void**)&DevicePathUtilitiesProtocol);
    if (status != EFI_SUCCESS) return status;

    EFI_DEVICE_PATH_TO_TEXT_PROTOCOL* DevicePathToTextProtocol;
    status = SystemTable->BootServices->LocateProtocol(&DevicePathToTextProtocolGuid, NULL, (void**)&DevicePathToTextProtocol);
    if (status != EFI_SUCCESS) return status;

    // Get device path referring to current image
    EFI_DEVICE_PATH_PROTOCOL* currentDevicePath;
    status = SystemTable->BootServices->HandleProtocol(ImageHandle, &LoadedImageDevicePathProtocolGuid, (void**)&currentDevicePath);
    if (status != EFI_SUCCESS) return status;

    CHAR16* devicePathString;
    devicePathString = DevicePathToTextProtocol->ConvertDevicePathToText(currentDevicePath, FALSE, TRUE);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Current device path: ");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, devicePathString);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");

    devicePathString = DevicePathToTextProtocol->ConvertDeviceNodeToText(currentDevicePath, FALSE, TRUE);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Current device node: ");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, devicePathString);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");

    // TODO: Make device path referring to app

    // Load app image
    EFI_DEVICE_PATH_PROTOCOL *appDevicePath = NULL; // TODO
    EFI_HANDLE appImageHandle;

    status = SystemTable->BootServices->LoadImage(TRUE, ImageHandle, appDevicePath, NULL, 0, &appImageHandle);
    if (status != EFI_SUCCESS) return status;

    // TODO: command-line arguments
    status = SystemTable->BootServices->StartImage(appImageHandle, NULL, NULL);

    SystemTable->BootServices->UnloadImage(appImageHandle);

    return status;
}

// EFI Image Entry Point
EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    (void)ImageHandle;  // Prevent compiler warning

    // Set text to yellow fg/ green bg
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_TEXT_ATTR(EFI_YELLOW,EFI_GREEN));

    // Clear screen to bg color
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Hello, World!\r\n\r\n");

    // Set text to red fg/ black bg
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_TEXT_ATTR(EFI_RED,EFI_BLACK));

    EFI_INPUT_KEY key;

    // Wait until keypress
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Press any key to launch app...\r\n");
    while (SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &key) != EFI_SUCCESS) ;

    // Restore to white fg, black bg, and clear screen
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_TEXT_ATTR(EFI_WHITE,EFI_BLACK));
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

    // Launch app, returns on error, or if app exits
    if (launch_app(ImageHandle, SystemTable) != EFI_SUCCESS) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, u"App failed to launch\r\n\r\n");
    }

    // Wait until keypress
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Press any key to shutdown...\r\n");
    while (SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &key) != EFI_SUCCESS) ;

    // Shutdown, does not return
    SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);

    // Should never get here
    return EFI_SUCCESS;
}
