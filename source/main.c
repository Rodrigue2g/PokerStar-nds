/*
 * Template Nintendo DS
 * May 2011
 */

#include <nds.h>
#include <stdio.h>

int main(void) {
	
    consoleDemoInit();
    //int a = 1;
    
    printf("\nWelcome to PokerStar!\n");

    while(1) {
        swiWaitForVBlank();	
    }
}
