/*
 * Template Nintendo DS
 * May 2011
 */

#include <nds.h>
#include <stdio.h>

int main(void) {
	
    consoleDemoInit();
    int a = 1;

    while(1){
        printf("\nHello Times:%d!\n",a++);
        swiWaitForVBlank();	
}
}
