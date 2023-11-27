/*
 * Template Nintendo DS
 * May 2011
 */

#include <nds.h>
#include <stdio.h>
//#include "graphics_sub.h"
#include "graphics_top.h"
#include "graphics_bottom.h"

//#include "bottom.h"
//#include "onec.h"


int main(int argc, char** argv) {

	//Configure and fill BG0
	configGraphics_Top();
    configGraphics_Bottom();

	//Position
    while(1){
        updateGraphics_Top();
    }
}
