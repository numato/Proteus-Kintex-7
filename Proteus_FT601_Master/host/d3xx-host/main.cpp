#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <conio.h>
#include <time.h>
#include "ftd3xx.h"

FT_DEVICE_LIST_INFO_NODE *devInfoList = NULL;
FT_CONFIGURATION_DESCRIPTOR conf_desc;
FT_INTERFACE_DESCRIPTOR intf_desc;
FT_DEVICE_DESCRIPTOR dev_desc;
FT_PIPE_INFORMATION p1inf;
FT_PIPE_INFORMATION p2inf;

FT_HANDLE handle = NULL;

DWORD NumDevs = 0;
DWORD BoardCount = 0;

uint8_t usb_mode = 0;
uint32_t test_size = 1024 * 1024 * 256;
uint32_t test_iters = 0;

void delay(uint32_t);
FT_STATUS run_loopback_test();
FT_STATUS run_source_test();

int main(void){
    FT_STATUS ftStatus;
    DWORD dev_count;
    DWORD driver_version;
    DWORD library_version;

    DWORD type;
    char serial[16];
    char description[64];
    DWORD id;
    DWORD flags;
    DWORD loc_id;

    printf("\r\nProteus K7 FT601 245 FIFO RX Test\r\n");
    printf("__________________________________\r\n\n");

    ftStatus = FT_GetLibraryVersion(&library_version);
    if (ftStatus != FT_OK){
        printf("[Error]: FT_STATUS: 0x%08x\r\n", ftStatus);
        return -1;
    } else {
        printf("\r\nFTDI D3XX Lib version = 0x%08x\n", library_version);
    }


    ftStatus = FT_ListDevices(&dev_count, NULL, FT_LIST_NUMBER_ONLY);
    if (ftStatus != FT_OK) {
        printf("[Error]: Couldn't get the number of devices connected. FT_STATUS: 0x%08x\r\n", ftStatus);
        return -1;
    } else {
        printf("Number of FTDI devices connected: %lu\r\n", dev_count);
    }


    if (dev_count > 0){
        ftStatus = FT_CreateDeviceInfoList(&dev_count);
        if (ftStatus != FT_OK){
            printf("[Error]: FT_STATUS: 0x%08x\r\n", ftStatus);
            return -1;
        }

        devInfoList = (FT_DEVICE_LIST_INFO_NODE*) malloc( sizeof(FT_DEVICE_LIST_INFO_NODE) * dev_count );
        if (devInfoList == NULL) {
            printf("[Error]: Memory allocation for device info list failed\r\n", ftStatus);
            return -1;
        }

        memset(devInfoList, 0, sizeof(FT_DEVICE_LIST_INFO_NODE) * dev_count);
        
        ftStatus = FT_GetDeviceInfoList(devInfoList, &dev_count);

        if (ftStatus == FT_OK) {
            printf("\r\n\nIndex\tSerial\t\tDescription\r\n\n");
            for (int i=0; i<dev_count; i++) {
                printf("%d\t%s\t\t%s\r\n", i, devInfoList[i].SerialNumber, devInfoList[i].Description);
            }
        } else{
            free(devInfoList);
            devInfoList = NULL;
            printf("[Error]: Failed to get device info list. FT_STATUS: 0x%08x\r\n", ftStatus);
            return -1;
        }

        int selectedIndex = 0;
        printf("\nEnter index to open:  ");
        scanf( "%d", &selectedIndex);
        //Consume the newline left in the buffer by scanf above
        getchar();

        

        ftStatus = FT_Create((ULONG *)selectedIndex, FT_OPEN_BY_INDEX, &handle);
        if (ftStatus != FT_OK){
            free(devInfoList);
            devInfoList = NULL;
            printf("[Error]: Failed to open device. FT_STATUS: 0x%08x\r\n", ftStatus);
            return -1;
        } else {
            printf("\r\nDevice opened : %d\r\n", selectedIndex);
            ftStatus = FT_GetDeviceInfoDetail(selectedIndex, &flags, &type, &id, &loc_id, serial, description, NULL);
            if (ftStatus == FT_OK) {
                printf("\tSerial     \t: %s\r\n", serial);
                printf("\tDescription\t: %s\r\n", description);
                printf("\tVID/PID    \t: %04X/%04X\r\n", id >> 16, id & 0x0000ffff);
                if (flags & FT_FLAGS_SUPERSPEED) {
                    usb_mode = 3;
                } else if (flags & FT_FLAGS_HISPEED) {
                    usb_mode = 2;
                }
                printf("\tUSB Mode   \t: %s\r\n", (usb_mode == 3) ? "USB 3.1 SuperSpeed" : "USB 2.0 HighSpeed");
            }

            ftStatus = FT_GetDriverVersion(handle, &driver_version);
            if (ftStatus == FT_OK) {
                printf("\tDriver     \t: 0x%08x\r\n", driver_version);
            }

            ftStatus = run_source_test();
            if (ftStatus != FT_OK) {
                free(devInfoList);
                devInfoList = NULL;
                FT_Close(handle);
                printf("[Error]: FT_STATUS: 0x%08x\r\n", ftStatus);
            }
        }
    }
    
    printf("\r\nProteus K7 FT601 245 FIFO RX Test Finished. Thank You!\r\n");
    FT_Close(handle);

    return 0;
}

void delay(uint32_t val) {
    for (uint32_t i=0; i<val; i++){
    }
}

FT_STATUS run_loopback_test() {
    
    FT_STATUS status = FT_OK;

    status = FT_GetDeviceDescriptor(handle, &dev_desc);
    if (FT_FAILED(status)) {
        printf("\r\n\n[Error]: Failed to get device descriptor. FT_STATUS: 0x%08x\r\n", status);
        return status;
    }
        
    
    status = FT_GetConfigurationDescriptor(handle, &conf_desc);
    if (FT_FAILED(status)){
        printf("\r\n\n[Error]: Failed to get configuration descriptor. FT_STATUS: 0x%08x\r\n", status);
        return status;
    } else{
        printf("\r\n\nNumber of interfaces: %u\r\n", conf_desc.bNumInterfaces);
    }
    

    status = FT_GetInterfaceDescriptor(handle, 1, &intf_desc);
    if (FT_FAILED(status)){
        printf("[Error]: Failed to get interface descriptor. FT_STATUS: 0x%08x\r\n", status);
        return status;
    } else{
        printf("Number of endpoints : %u\r\n", intf_desc.bNumEndpoints);
    }

    status = FT_GetPipeInformation(handle, 1, 0, &p1inf);
    if (FT_FAILED(status)){
        printf("[Error]: Failed to get pipe-0 information. FT_STATUS: 0x%08x\r\n", status);
        return status;
    } else{
        printf("Endpoint (addr)     : 0-OUT (0x%02X)\r\n", p1inf.PipeId);
    }

    status = FT_GetPipeInformation(handle, 1, 1, &p2inf);
    if (FT_FAILED(status)){
        printf("[Error]: Failed to get pipe-1 information. FT_STATUS: 0x%08x\r\n", status);
        return status;
    } else{
       printf("Endpoint (addr)     : 1-IN  (0x%02X)\r\n",  p2inf.PipeId);
    }

    ULONG pipe_timeout = 5000;
    status = FT_GetPipeTimeout(handle, p1inf.PipeId, &pipe_timeout);
    if (FT_FAILED(status)){
        printf("[Error]: Failed to set pipe-0 timeout. FT_STATUS: 0x%08x\r\n", status);
        return status;
    }

    status = FT_GetPipeTimeout(handle, p2inf.PipeId, &pipe_timeout);
    if (FT_FAILED(status)){
        printf("[Error]: Failed to set pipe-1 timeout. FT_STATUS: 0x%08x\r\n", status);
        return status;
    }

    printf("\r\nRunning Loopback Test\r\n\n");

    return status;
}

FT_STATUS run_source_test() {
    
    FT_STATUS status = FT_OK;

    status = FT_GetDeviceDescriptor(handle, &dev_desc);
    if (FT_FAILED(status)) {
        printf("\r\n\n[Error]: Failed to get device descriptor. FT_STATUS: 0x%08x\r\n", status);
        return status;
    }
        
    
        status = FT_GetConfigurationDescriptor(handle, &conf_desc);
    if (FT_FAILED(status)){
        printf("\r\n\n[Error]: Failed to get configuration descriptor. FT_STATUS: 0x%08x\r\n", status);
        return status;
    } else{
        printf("\r\n\nNumber of interfaces: %u\r\n", conf_desc.bNumInterfaces);
    }
    

    status = FT_GetInterfaceDescriptor(handle, 1, &intf_desc);
    if (FT_FAILED(status)){
        printf("[Error]: Failed to get interface descriptor. FT_STATUS: 0x%08x\r\n", status);
        return status;
    } else{
        printf("Number of endpoints : %u\r\n", intf_desc.bNumEndpoints);
    }

    status = FT_GetPipeInformation(handle, 1, 0, &p1inf);
    if (FT_FAILED(status)){
        printf("[Error]: Failed to get pipe-0 information. FT_STATUS: 0x%08x\r\n", status);
        return status;
    } else{
        printf("Endpoint (addr)     : 0-OUT (0x%02X)\r\n", p1inf.PipeId);
    }

    status = FT_GetPipeInformation(handle, 1, 1, &p2inf);
    if (FT_FAILED(status)){
        printf("[Error]: Failed to get pipe-1 information. FT_STATUS: 0x%08x\r\n", status);
        return status;
    } else{
       printf("Endpoint (addr)     : 1-IN  (0x%02X)\r\n",  p2inf.PipeId);
    }

    ULONG pipe_timeout = 5000;
    status = FT_GetPipeTimeout(handle, p1inf.PipeId, &pipe_timeout);
    if (FT_FAILED(status)){
        printf("[Error]: Failed to set pipe-0 timeout. FT_STATUS: 0x%08x\r\n", status);
        return status;
    }

    status = FT_GetPipeTimeout(handle, p2inf.PipeId, &pipe_timeout);
    if (FT_FAILED(status)){
        printf("[Error]: Failed to set pipe-1 timeout. FT_STATUS: 0x%08x\r\n", status);
        return status;
    }

    printf("\r\nRunning Source Test\r\n\n");

    printf("%s Mode\r\n\n", (usb_mode == 3) ? "USB 3.1 SuperSpeed" : "USB 2.0 HighSpeed");
    if (usb_mode == 3) {
        printf("Testing with 16 GB of data\r\n");
        test_iters = 64;  // 64 * 256 MB -> 16 GB
    } else {
        printf("Testing with 256 MB of data\r\n");
        test_iters = 1;
    }

    uint32_t* rx_buf = (uint32_t *) malloc(test_size);
    if (rx_buf == NULL) {
        printf("[Error]: Failed to reserve 256 MB of memory. Please check free memory\r\n", status);
        return FT_OTHER_ERROR;
    }

    memset(rx_buf, 0, test_size);

    clock_t tstart, tend;
    double cpu_time_used;
    uint64_t total_clocks = 0;
    uint32_t bytes_rx = 0;
    uint32_t start = 0x00;
    uint8_t error = 0;
    for (int i=0; i < test_iters; i++) {
        error = 0;
        tstart = clock();
        status = FT_ReadPipe(handle, p2inf.PipeId, (PUCHAR) rx_buf, test_size, (PULONG) &bytes_rx, NULL);
        tend = clock();
        total_clocks += (tend - tstart);
        cpu_time_used = ((double) total_clocks ) / CLOCKS_PER_SEC;

        if (FT_FAILED(status) || (bytes_rx != test_size)) {
            printf("[Error]: Failed to read 256 MB chunk of data from FT601 device. Read: 0x%08x bytes\r\n", status, bytes_rx);
            free(rx_buf);
            return status;
        }

        start = rx_buf[0];
        printf("[%02d] 256MB chunk: 0x%08x - 0x%08x, verifying...", i, rx_buf[0], rx_buf[(test_size / 4) - 1]);
        for (uint32_t j = 0; j < test_size / 4; j++) {
            if (rx_buf[j] != (start + j) % 0x100000000) {
                error = 1;
                break;
            }
        }

        if (error) {
            printf("FAILED!\r\n");
            printf("[Error] Verification failed...\r\n");
            free(rx_buf);
            return FT_OTHER_ERROR;
        } else {
            printf("ok\r\n");
        }
    }
    free(rx_buf);
    printf("\nTransfer time for %s of data: %.02f seconds\r\n", (usb_mode == 3) ? "16 GB" : "256 MB", cpu_time_used);
    printf("Transfer rate: %.02f MB/s\r\n", ((double) test_size / 1024.0f / 1024.0f * test_iters) / cpu_time_used);
    return status;
}