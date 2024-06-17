#include <stdio.h>

#include "matrix.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "main.h"



/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <string.h>
#include "matrix.h"
#include "time.h"



int main(void) {

    printf("Hello, Entering App !\n");

    app();


    char buffer[20];

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */

    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        if (kindex >= 198){
            for (int amostra = 0; amostra < 198; amostra++)
            {
                sprintf(buffer, "%d, %d;", ialfa[amostra], ibeta[amostra]);
                CDC_Transmit_FS((uint8_t*) buffer, strlen(buffer));
                HAL_Delay(2);
            }

            while(1) {};

        }


    }
    return 0;
}
