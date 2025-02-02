// Res:
// 	Old PSX dev tutorials ; http://lameguy64.net/svn/pstutorials/chapter1/1-display.html
//	nolibgs xa docs ; https://github.com/ABelliqueux/nolibgs_hello_worlds/wiki/XA
//	Archerite's tile drawing code ; https://discord.com/channels/642647820683444236/642849069378568192/1128603597546995744

#include <sys/types.h>
#include <stdio.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libcd.h>
#include <libspu.h>
#include <libmath.h>

#include "textures.h"

#include "types.h"
#include "tims.h"
#include "levels.h"
#include "LUT.h"

#define VMODE		0	// Video Mode ; 0: NTSC, 1: PAL
#define SCREENXRES	320	// Screen width
#define SCREENYRES	240	// Screen height

#define FONTSIZE	8 * 7	// Text Field Height

#define OTLEN	180	// Length of ordering table.
					// 16x11 = 176 possible tiles + paddle and 3 balls.

SpuCommonAttr spuSettings;	// SPU attributes.

#define CD_SECTOR_SIZE	2048	// Unsure what this is.

DISPENV disp[2];	// Double buffered DISPENV and DRAWENV
DRAWENV draw[2];

u_long ot[2][OTLEN];	// Double-buffered ordering table.
						// This is used to queue primitives for drawing.

char primbuff[2][32768];		// Double primitive buffer.
char* nextpri = primbuff[0];	// Pointer to the next primitive in primbuff.
short db = 0;	// Which buffer to use.

void init(void){
    ResetGraph(0);														// Initialize drawing engine with a complete reset (0)
    SetDefDispEnv(&disp[0], 0, 0, SCREENXRES, SCREENYRES);				// Set display area for both &disp[0] and &disp[1]
    SetDefDispEnv(&disp[1], 0, SCREENYRES, SCREENXRES, SCREENYRES);     // &disp[0] is on top  of &disp[1]
    SetDefDrawEnv(&draw[0], 0, SCREENYRES, SCREENXRES, SCREENYRES);     // Set draw for both &draw[0] and &draw[1]
    SetDefDrawEnv(&draw[1], 0, 0, SCREENXRES, SCREENYRES);				// &draw[0] is below &draw[1]
    
	// Check if PAL.
	if (VMODE){
        SetVideoMode(MODE_PAL);
        disp[0].screen.y += 8;	// add offset ; 240 + 8 + 8 = 256
        disp[1].screen.y += 8;
    }
	
    SetDispMask(1);                 // Display on screen
	
    setRGB0(&draw[0], 50, 50, 50);	// set color for first draw area
    setRGB0(&draw[1], 50, 50, 50);	// set color for second draw area
    
	draw[0].isbg = 1;               // set mask for draw areas. 1 means repainting the area with the RGB color each frame 
    draw[1].isbg = 1;
    
	PutDispEnv(&disp[db]);          // set the disp and draw environnments
    PutDrawEnv(&draw[db]);
    
	FntLoad(960, 0);                // Load font to vram at 960,0(+128)
    FntOpen(0, 0, SCREENXRES - MARGINX * 2, FONTSIZE, 0, 280 ); // FntOpen(x, y, width, height,  black_bg, max. nbr. chars
	
	SpuInit();
}

void display(void){
    DrawSync(0);                    // Wait for all drawing to terminate
    VSync(0);                       // Wait for the next vertical blank
    PutDispEnv(&disp[db]);          // set alternate disp and draw environnments
    PutDrawEnv(&draw[db]);
	
	// Draw everything in ordering table.
	// Since a reverse ot is used, the last element is drawn first.
	// Idk what dictates what ot to use.
	DrawOTag(&ot[db][OTLEN - 1]);
	
	// Switch buffers.
    db = !db;
	nextpri = primbuff[db];
}

int CheckOverlap(Vec4i r1, Vec4i r2){
	if(r1.x < r2.x+r2.w && r1.x+r1.w >= r2.x &&
		r1.y < r2.y+r2.h && r1.y+r1.h >= r2.y){
		
		return 1;
	}
	
	return 0;
}

int main(void){
    init();
	
	// Init gamepad.
	int pad = 0;
	PadInit(0);
	
	while (1){
        // Clear reversed ordering table.
		ClearOTagR(ot[db], OTLEN);
		
		// Poll gamepad.
		pad = PadRead(0);
	
		FntFlush(-1);	// Draw print stream, clear print buffer.
		display();
    }
	
    return 0;
}