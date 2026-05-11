/*
 * fatfs_sd.c
 *
 *  Created on: May 12, 2026
 *      Author: Samkelo Nkabinde
 */

#include "fatfs_sd.h"

// SD Card Commands
#define CMD0    (0)         // GO_IDLE_STATE
#define CMD8    (8)         // SEND_IF_COND
#define CMD17   (17)        // READ_SINGLE_BLOCK
#define CMD24   (24)        // WRITE_BLOCK
#define CMD55   (55)        // APP_CMD
#define CMD58   (58)        // READ_OCR
#define ACMD41  (41)        // APP_SEND_OP_COND

static volatile DSTATUS Stat = STA_NOINIT;
static uint8_t CardType;

// --- Low Level SPI Functions ---
static void SD_CS_High(void) { HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_SET); }
static void SD_CS_Low(void)  { HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_RESET); }

static uint8_t SPI_RxByte(void) {
    uint8_t dummy = 0xFF, data = 0;
    HAL_SPI_TransmitReceive(HSPI_SD, &dummy, &data, 1, 10);
    return data;
}

static void SPI_TxByte(uint8_t data) {
    HAL_SPI_Transmit(HSPI_SD, &data, 1, 10);
}

// Send a command to the SD card
static uint8_t SD_SendCmd(uint8_t cmd, uint32_t arg) {
    uint8_t res, n;
    if (cmd & 0x80) {
        cmd &= 0x7F;
        res = SD_SendCmd(CMD55, 0);
        if (res > 1) return res;
    }
    SD_CS_Low();
    SPI_TxByte(0xFF);
    SPI_TxByte(cmd | 0x40);
    SPI_TxByte((uint8_t)(arg >> 24));
    SPI_TxByte((uint8_t)(arg >> 16));
    SPI_TxByte((uint8_t)(arg >> 8));
    SPI_TxByte((uint8_t)arg);
    n = 0x01;
    if (cmd == CMD0) n = 0x95;
    if (cmd == CMD8) n = 0x87;
    SPI_TxByte(n);
    n = 10;
    do {
        res = SPI_RxByte();
    } while ((res & 0x80) && --n);
    return res;
}

// --- FatFs Interface Functions ---

DSTATUS SD_disk_initialize(BYTE pdrv) {
    uint8_t n, type, ocr[4];
    if (pdrv) return STA_NOINIT;

    // Give SD card time to wake up
    HAL_Delay(10);
    for (n = 0; n < 10; n++) SPI_TxByte(0xFF);

    if (SD_SendCmd(CMD0, 0) == 1) { // Enter Idle state
        if (SD_SendCmd(CMD8, 0x1AA) == 1) { // SDv2
            for (n = 0; n < 4; n++) ocr[n] = SPI_RxByte();
            if (ocr[2] == 0x01 && ocr[3] == 0xAA) {
                            // ADDED TIMEOUT PROTECTION
                            uint32_t timeout = HAL_GetTick() + 1000; // 1 second maximum wait
                            while (SD_SendCmd(ACMD41, 1UL << 30)) {
                                if (HAL_GetTick() > timeout) break; // Break the loop if it takes too long
                            }

                            if (SD_SendCmd(CMD58, 0) == 0) {
                    for (n = 0; n < 4; n++) ocr[n] = SPI_RxByte();
                    type = (ocr[0] & 0x40) ? 3 : 2; // SDHC/SDXC or SDv2
                }
            }
        }
    }
    CardType = type;
    SD_CS_High();
    SPI_RxByte();

    if (type) {
        Stat &= ~STA_NOINIT; // Initialization succeeded
    } else {
        Stat |= STA_NOINIT;  // Initialization failed
    }
    return Stat;
}

DSTATUS SD_disk_status(BYTE pdrv) {
    if (pdrv) return STA_NOINIT;
    return Stat;
}

DRESULT SD_disk_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count) {
    if (pdrv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    if (!(CardType & 2)) sector *= 512; // Convert to byte address if not SDHC

    if (count == 1) {
        if (SD_SendCmd(CMD17, sector) == 0) { // READ_SINGLE_BLOCK
            while (SPI_RxByte() != 0xFE); // Wait for data token
            for (uint16_t i = 0; i < 512; i++) buff[i] = SPI_RxByte();
            SPI_RxByte(); SPI_RxByte(); // Discard CRC
            SD_CS_High();
            return RES_OK;
        }
    }
    SD_CS_High();
    return RES_ERROR;
}

DRESULT SD_disk_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count) {
    if (pdrv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    if (!(CardType & 2)) sector *= 512;

    if (count == 1) {
        if (SD_SendCmd(CMD24, sector) == 0) { // WRITE_BLOCK
            SPI_TxByte(0xFF);
            SPI_TxByte(0xFE); // Data token
            for (uint16_t i = 0; i < 512; i++) SPI_TxByte(buff[i]);
            SPI_TxByte(0xFF); SPI_TxByte(0xFF); // Dummy CRC
            if ((SPI_RxByte() & 0x1F) == 0x05) { // Data accepted
                while (SPI_RxByte() == 0); // Wait while busy
                SD_CS_High();
                return RES_OK;
            }
        }
    }
    SD_CS_High();
    return RES_ERROR;
}

DRESULT SD_disk_ioctl(BYTE pdrv, BYTE cmd, void* buff) {
    if (pdrv) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    DRESULT res = RES_ERROR;
    switch (cmd) {
        case CTRL_SYNC: // Wait for write process to end
            SD_CS_Low();
            if (SPI_RxByte() == 0xFF) res = RES_OK;
            SD_CS_High();
            break;
        case GET_SECTOR_COUNT:
        case GET_SECTOR_SIZE:
        case GET_BLOCK_SIZE:
            // Minimal implementation usually returns OK for basic FAT32 setups
            res = RES_OK;
            break;
    }
    return res;
}
