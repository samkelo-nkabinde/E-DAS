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
#define ACMD41  (0x80 + 41)       // APP_SEND_OP_COND


static volatile DSTATUS Stat = STA_NOINIT;
static uint8_t CardType;

// --- Low Level SPI Functions ---
static void SD_CS_High(void) { HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_SET); }
static void SD_CS_Low(void)  { HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_RESET); }

static uint8_t SPI_TxRxByte(uint8_t data)
{
    uint8_t rx = 0x00;

    HAL_SPI_TransmitReceive(HSPI_SD, &data, &rx, 1, HAL_MAX_DELAY);

    return rx;
}

static uint8_t SPI_RxByte(void)
{
    return SPI_TxRxByte(0xFF);
}

static void SPI_TxByte(uint8_t data)
{
    SPI_TxRxByte(data);
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

DSTATUS SD_disk_initialize(BYTE pdrv)
{
    uint8_t n;
    uint8_t type = 0;
    uint8_t ocr[4];
    uint8_t response;
    uint32_t timeout;

    if (pdrv)
    {
        return STA_NOINIT;
    }

    Stat = STA_NOINIT;
    CardType = 0;

    SD_CS_High();
    HAL_Delay(10);

    // Send at least 74 clock pulses with CS high
    for (n = 0; n < 10; n++)
    {
        SPI_TxByte(0xFF);
    }

    SD_CS_High();
    HAL_Delay(10);

   // SD_DebugPrint("CS high idle", SPI_RxByte());

    SD_CS_Low();
    HAL_Delay(1);
  //  SD_DebugPrint("CS low idle", SPI_RxByte());
    SD_CS_High();
    SPI_RxByte();

    // CMD0: enter idle state
    response = SD_SendCmd(CMD0, 0);
   // SD_DebugPrint("CMD0", response);
    SD_CS_High();
    SPI_RxByte();

    if (response != 0x01)
    {
        return STA_NOINIT;
    }

    // CMD8: check SD v2 card
    response = SD_SendCmd(CMD8, 0x000001AA);

    for (n = 0; n < 4; n++)
    {
        ocr[n] = SPI_RxByte();
    }
   // SD_DebugPrint("CMD8", response);
    SD_CS_High();
    SPI_RxByte();

    if (response == 0x01 && ocr[2] == 0x01 && ocr[3] == 0xAA)
    {
        // SD v2 card
        timeout = HAL_GetTick() + 1000;

        do
        {
            response = SD_SendCmd(ACMD41, 1UL << 30);
            SD_CS_High();
            SPI_RxByte();

            if (HAL_GetTick() > timeout)
            {
                return STA_NOINIT;
            }

        } while (response != 0x00);

        // CMD58: read OCR
        response = SD_SendCmd(CMD58, 0);

        for (n = 0; n < 4; n++)
        {
            ocr[n] = SPI_RxByte();
        }
     //   SD_DebugPrint("CMD58", response);
        SD_CS_High();
        SPI_RxByte();

        if (response != 0x00)
        {
            return STA_NOINIT;
        }

        if (ocr[0] & 0x40)
        {
            type = 3;   // SDHC or SDXC
        }
        else
        {
            type = 2;   // SDSC v2
        }
    }
    else
    {
        // For now, only support SD v2 cards
        return STA_NOINIT;
    }

    CardType = type;

    if (CardType)
    {
        Stat &= ~STA_NOINIT;
    }
    else
    {
        Stat = STA_NOINIT;
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

DRESULT SD_disk_ioctl(BYTE pdrv, BYTE cmd, void* buff)
{
    if (pdrv)
    {
        return RES_PARERR;
    }

    if (Stat & STA_NOINIT)
    {
        return RES_NOTRDY;
    }

    switch (cmd)
    {
        case CTRL_SYNC:
            SD_CS_Low();

            if (SPI_RxByte() == 0xFF)
            {
                SD_CS_High();
                return RES_OK;
            }

            SD_CS_High();
            return RES_ERROR;

        case GET_SECTOR_SIZE:
            *(WORD *)buff = 512;
            return RES_OK;

        case GET_BLOCK_SIZE:
            *(DWORD *)buff = 1;
            return RES_OK;

        case GET_SECTOR_COUNT:
            *(DWORD *)buff = 8000000;   // temporary value for mount test
            return RES_OK;

        default:
            return RES_PARERR;
    }
}
