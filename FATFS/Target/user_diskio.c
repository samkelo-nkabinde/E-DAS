/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    user_diskio.c
  * @brief   This file includes a diskio driver skeleton to be completed by the user.
  ******************************************************************************
  */
/* USER CODE END Header */

/* USER CODE BEGIN DECL */
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "ff_gen_drv.h"

// ADD THIS: Include the SD driver header
#include "fatfs_sd.h"

/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;
/* USER CODE END DECL */

/* Private function prototypes -----------------------------------------------*/
DSTATUS USER_initialize (BYTE pdrv);
DSTATUS USER_status (BYTE pdrv);
DRESULT USER_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
#if _USE_WRITE == 1
  DRESULT USER_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT USER_ioctl (BYTE pdrv, BYTE cmd, void *buff);
#endif /* _USE_IOCTL == 1 */

Diskio_drvTypeDef  USER_Driver =
{
  USER_initialize,
  USER_status,
  USER_read,
#if  _USE_WRITE
  USER_write,
#endif  /* _USE_WRITE == 1 */
#if  _USE_IOCTL == 1
  USER_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

DSTATUS USER_initialize (BYTE pdrv)
{
  /* USER CODE BEGIN INIT */
    // Call the external driver's initialize function
    Stat = SD_disk_initialize(pdrv);
    return Stat;
  /* USER CODE END INIT */
}

DSTATUS USER_status (BYTE pdrv)
{
  /* USER CODE BEGIN STATUS */
    // Call the external driver's status function
    Stat = SD_disk_status(pdrv);
    return Stat;
  /* USER CODE END STATUS */
}

DRESULT USER_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
  /* USER CODE BEGIN READ */
    // Call the external driver's read function
    return SD_disk_read(pdrv, buff, sector, count);
  /* USER CODE END READ */
}

#if _USE_WRITE == 1
DRESULT USER_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
  /* USER CODE BEGIN WRITE */
    // Call the external driver's write function
    return SD_disk_write(pdrv, buff, sector, count);
  /* USER CODE END WRITE */
}
#endif /* _USE_WRITE == 1 */

#if _USE_IOCTL == 1
DRESULT USER_ioctl (BYTE pdrv, BYTE cmd, void *buff)
{
  /* USER CODE BEGIN IOCTL */
    // Call the external driver's ioctl function
    return SD_disk_ioctl(pdrv, cmd, buff);
  /* USER CODE END IOCTL */
}
#endif /* _USE_IOCTL == 1 */
