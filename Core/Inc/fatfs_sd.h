/*
 * fatfs_sd.h
 *
 *  Created on: May 12, 2026
 *      Author: 28118944
 */

#ifndef INC_FATFS_SD_H_
#define INC_FATFS_SD_H_

#include "main.h"
#include "diskio.h"

// Define the SPI Handle and CS Pin based on your main.c
extern SPI_HandleTypeDef hspi1;
#define HSPI_SD &hspi1
#define SD_CS_PORT GPIOA
#define SD_CS_PIN GPIO_PIN_15

// Function Prototypes for user_diskio.c to call
DSTATUS SD_disk_initialize (BYTE pdrv);
DSTATUS SD_disk_status (BYTE pdrv);
DRESULT SD_disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
DRESULT SD_disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
DRESULT SD_disk_ioctl (BYTE pdrv, BYTE cmd, void* buff);

#endif /* INC_FATFS_SD_H_ */
