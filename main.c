#include <stdio.h>

#include "matrix.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <string.h>
#include "matrix.h"
#include "time.h"
#include "usbd_cdc_if.h"

int main(void) {
    app();
    printf("Hello, World!\n");
    return 0;
}
