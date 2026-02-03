#include "xil_printf.h"

#define LEDS ( *(unsigned volatile *) 0x40000008)
#define SW ( *(unsigned volatile *) 0x40000000)
#define JB (*(unsigned volatile *) 0x40010000)

void delay_ms(unsigned t);
void increment(int *oneHZCntr, int *tenHZCntr, int *twentyHZCntr, int *fiveSecCntr);
void squareWave (int *tenHZCntr, int *twentyHZCntr);
void turnOnLEDs(int *oneHZCntr);
void fiveSecondPrint(int *fiveSecCntr);



int main()
{
    xil_printf("Start \n");
    
    int oneHZCntr = 0;
    int tenHZCntr = 0;
    int twentyHZCntr = 0;
    int fiveSecCntr = 0;
    
    while(1){
        
        delay_ms(1);
        
        increment(&oneHZCntr, &tenHZCntr, &twentyHZCntr, &fiveSecCntr);

        squareWave(&tenHZCntr, &twentyHZCntr);
        
        turnOnLEDs(&oneHZCntr);

        fiveSecondPrint(&fiveSecCntr);
    }

   return 0;

}


void delay_ms(unsigned t){
    for( int i=0;i<3000;i++){
        for(unsigned int j = 0; j<t ;j++){}  
    }    
}

void increment(int *oneHZCntr, int *tenHZCntr, int *twentyHZCntr, int *fiveSecCntr){
    (*tenHZCntr)++;
    (*twentyHZCntr)++;
    (*fiveSecCntr)++;
    (*oneHZCntr)++;
}

void squareWave(int *tenHZCntr, int *twentyHZCntr){
    //Square Wave of 10Hz has a period of 100ms
        if ((*tenHZCntr) >= 50){
            JB ^= 0x01;
            *tenHZCntr = 0;
        }
        //Square Wave of 20Hz has a period of 50ms
        if ((*twentyHZCntr) >= 25){
            JB ^= 0x02;
            *twentyHZCntr = 0;
        }
}

void turnOnLEDs(int *oneHZCntr){
    //Switches 15 & 14
    if ((SW & 0xC000) >> 14 == 0){
        LEDS = 0x00000000;
    } else if ((SW & 0xC000) >> 14 == 1){
        LEDS = 0xFFFFFFFF;
    } else if ((SW & 0xC000) >> 14 == 2){
        LEDS = SW & 0xFFFFFFFF;
    } else if((SW & 0xC000) >> 14 == 3){
        //Square Wave of 1Hz has a period of 1000ms
        if ((*oneHZCntr) >= 500){
            LEDS ^= 0xFFFFFFFF;
            *oneHZCntr = 0;
        }
    }
}

void fiveSecondPrint(int *fiveSecCntr){
    //Prints Every 5 Seconds
    if ((*fiveSecCntr) >= 5000){
        xil_printf("0x%04X\r\n", SW);
        *fiveSecCntr = 0;
    }
}


