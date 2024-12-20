#include "packman.h"
#include "../GLCD/GLCD.h"

// DEFINING THE SCORES AND NUMBER OF PILLS 
#define STDSCORE 10
#define POWERSCORE 50
#define N_STDPILLS 240

// Assigning to the pills the associated score which contributes to increment the total one

void init_GameSpace(powerpill *pw){
	
	GUI_Text(10,10, (unsigned char*) "Game over over in",0xffff,Black); 
	GUI_Text(180,10, (unsigned char*) pw->score,0xffff,Black); 

}

void init_Pills(powerpill *pw, stdpill *std){
	
	std->score=STDSCORE;
	pw->score=POWERSCORE;
}
