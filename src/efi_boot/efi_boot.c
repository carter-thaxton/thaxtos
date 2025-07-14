#include <efi/efi.h>

EFI_GUID DevicePathProtocolGuid = EFI_DEVICE_PATH_PROTOCOL_GUID;
EFI_GUID DevicePathUtilitiesProtocolGuid = EFI_DEVICE_PATH_UTILITIES_PROTOCOL_GUID;
EFI_GUID DevicePathToTextProtocolGuid = EFI_DEVICE_PATH_TO_TEXT_PROTOCOL_GUID;
EFI_GUID DevicePathFromTextProtocolGuid = EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL_GUID;
EFI_GUID LoadedImageDevicePathProtocolGuid = EFI_LOADED_IMAGE_DEVICE_PATH_PROTOCOL_GUID;
EFI_GUID LoadedImageProtocolGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
EFI_GUID SimpleFileSystemProtocolGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

EFI_HANDLE ImageHandle;
EFI_SYSTEM_TABLE* SystemTable;
EFI_DEVICE_PATH_UTILITIES_PROTOCOL* DevicePathUtilitiesProtocol;
EFI_DEVICE_PATH_TO_TEXT_PROTOCOL* DevicePathToTextProtocol;
EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL* DevicePathFromTextProtocol;
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* SimpleFileSystemProtocol;

CONST EFI_DEVICE_PATH_PROTOCOL EndDevicePath = {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
        END_DEVICE_PATH_LENGTH,
        0
    }
};

CONST CHAR16* kernel_path = u"vmlinux";
CONST CHAR16* cmdline_path = u"cmdline";


void print(CONST CHAR16* str);

void* memcpy(void* dst, const void* src, UINTN n) {
    UINT8* d = (UINT8*) dst;
    CONST UINT8* s = (CONST UINT8*) src;

    while (n--) {
        *d++ = *s++;
    }
    return d;
}

void* malloc(UINTN size) {
    void* result;
    EFI_STATUS status = SystemTable->BootServices->AllocatePool(EfiLoaderData, size, &result);
    if (status != EFI_SUCCESS) {
        print(u"Could not allocate memory!\r\n");
    }
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

void clear_screen() {
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
}

void print(CONST CHAR16* str) {
    SystemTable->ConOut->OutputString(SystemTable->ConOut, (CHAR16*) str);
}

void print_device_path(EFI_DEVICE_PATH_PROTOCOL* devicePath) {
    CHAR16* devicePathString;
    devicePathString = DevicePathToTextProtocol->ConvertDevicePathToText(devicePath, FALSE, TRUE);
    print(devicePathString);
    free(devicePathString);
}

EFI_DEVICE_PATH_PROTOCOL* DevicePathFromHandle(EFI_HANDLE Device) {
    EFI_DEVICE_PATH_PROTOCOL* DevicePath = NULL;
    SystemTable->BootServices->HandleProtocol(Device, &DevicePathProtocolGuid, (void**)&DevicePath);
    return DevicePath;
}

EFI_DEVICE_PATH_PROTOCOL* FileDevicePath(EFI_HANDLE Device, CONST CHAR16* FileName) {
    FILEPATH_DEVICE_PATH      *FilePath;
    EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
    EFI_DEVICE_PATH_PROTOCOL  *FileDevicePath;

    DevicePath = NULL;

    UINTN Size = (strlen(FileName) + 1) * sizeof(CHAR16);
    FileDevicePath = malloc(Size + SIZE_OF_FILEPATH_DEVICE_PATH + END_DEVICE_PATH_LENGTH);
    if (FileDevicePath != NULL) {
        FilePath = (FILEPATH_DEVICE_PATH *) FileDevicePath;
        FilePath->Header.Type    = MEDIA_DEVICE_PATH;
        FilePath->Header.SubType = MEDIA_FILEPATH_DP;
        memcpy(&FilePath->PathName, FileName, Size);
        SetDevicePathNodeLength(&FilePath->Header, Size + SIZE_OF_FILEPATH_DEVICE_PATH);
        SetDevicePathEndNode(NextDevicePathNode(&FilePath->Header));

        if (Device != NULL) {
            DevicePath = DevicePathFromHandle(Device);
        }

        DevicePath = DevicePathUtilitiesProtocol->AppendDevicePath(DevicePath, FileDevicePath);
        free(FileDevicePath);
    }

    return DevicePath;
}

EFI_INPUT_KEY wait_for_key() {
    EFI_INPUT_KEY key;
    while (SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &key) != EFI_SUCCESS) ;
    return key;
}

EFI_STATUS launch_app(EFI_DEVICE_PATH_PROTOCOL* appDevicePath, CHAR16* command_line) {
    EFI_STATUS status;

    // Load app image
    EFI_HANDLE appImageHandle;
    status = SystemTable->BootServices->LoadImage(TRUE, ImageHandle, appDevicePath, NULL, 0, &appImageHandle);
    if (status != EFI_SUCCESS) return status;

    EFI_LOADED_IMAGE_PROTOCOL* appImageProtocol;
    status = SystemTable->BootServices->HandleProtocol(appImageHandle, &LoadedImageProtocolGuid, (void**)&appImageProtocol);
    if (status != EFI_SUCCESS) return status;

    // Set command-line arguments
    appImageProtocol->LoadOptions = command_line;
    appImageProtocol->LoadOptionsSize = strlen(command_line) * sizeof(CHAR16);

    status = SystemTable->BootServices->StartImage(appImageHandle, NULL, NULL);

    SystemTable->BootServices->UnloadImage(appImageHandle);

    return status;
}

// EFI Image Entry Point
EFI_STATUS EFIAPI efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable) {
    EFI_STATUS status;

    // init globals
    ImageHandle = imageHandle;
    SystemTable = systemTable;

    // init protocols
    status = SystemTable->BootServices->LocateProtocol(&DevicePathUtilitiesProtocolGuid, NULL, (void**)&DevicePathUtilitiesProtocol);
    if (status != EFI_SUCCESS) return status;

    status = SystemTable->BootServices->LocateProtocol(&DevicePathToTextProtocolGuid, NULL, (void**)&DevicePathToTextProtocol);
    if (status != EFI_SUCCESS) return status;

    status = SystemTable->BootServices->LocateProtocol(&DevicePathFromTextProtocolGuid, NULL, (void**)&DevicePathFromTextProtocol);
    if (status != EFI_SUCCESS) return status;

    // Get root ESP filesystem, where curent image is loaded
    EFI_LOADED_IMAGE_PROTOCOL* currentImage;
    status = SystemTable->BootServices->HandleProtocol(ImageHandle, &LoadedImageProtocolGuid, (void**)&currentImage);
    if (status != EFI_SUCCESS) return status;

    EFI_HANDLE rootDevice = currentImage->DeviceHandle;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fileSystemProtocol;
    status = SystemTable->BootServices->HandleProtocol(rootDevice, &SimpleFileSystemProtocolGuid, (void**)&fileSystemProtocol);
    if (status != EFI_SUCCESS) return status;

    EFI_FILE_PROTOCOL* rootDir;
    status = fileSystemProtocol->OpenVolume(fileSystemProtocol, &rootDir);
    if (status != EFI_SUCCESS) return status;

    // load cmdline file into buffer
    CHAR16 command_line_16[1024];
    command_line_16[0] = u'\0';

    EFI_FILE_PROTOCOL* cmdlineFile;
    status = rootDir->Open(rootDir, &cmdlineFile, (CHAR16*)cmdline_path, EFI_FILE_MODE_READ, 0);
    if (status == EFI_SUCCESS) {
        CHAR8 command_line[512];
        UINTN command_line_size = sizeof(command_line);
        command_line[command_line_size-1] = '\0';
        status = cmdlineFile->Read(cmdlineFile, &command_line_size, command_line);

        if (status == EFI_SUCCESS) {
            // convert to UTF-16
            UINTN j;
            for (j=0; command_line[j] != '\0' && command_line[j] != '\n'; j++) {
                command_line_16[j] = command_line[j];
            }
            command_line_16[j] = u'\0';
        }

        cmdlineFile->Close(cmdlineFile);
    }

    // create path to kernel
    EFI_DEVICE_PATH_PROTOCOL* kernelDevicePath = FileDevicePath(rootDevice, kernel_path);

    clear_screen();
    print(u"Launching: ");
    print_device_path(kernelDevicePath);
    print(u" ");
    print(command_line_16);
    print(u"\r\n");
    // wait_for_key();

    // Launch kernel, returns on error, or if app exits
    if (launch_app(kernelDevicePath, command_line_16) != EFI_SUCCESS) {
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
