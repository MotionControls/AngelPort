#ifndef TEXTURES_H
#define TEXTURES_H

#include <libgpu.h>

extern unsigned long _binary_res_objs1_tim_start[];
extern unsigned long _binary_res_objs2_tim_start[];
extern unsigned long _binary_res_objs3_tim_start[];
extern unsigned long _binary_res_objs4_tim_start[];

TIM_IMAGE timObjs1;
TIM_IMAGE timObjs2;
TIM_IMAGE timObjs3;
TIM_IMAGE timObjs4;

void LoadTexture(u_long* tim, TIM_IMAGE* tparam){	// Credit: Lameguy64 ; lameguy64.net/svn/pstutorials/chapter1/3-textures.html
	OpenTIM(tim);		// Opens the TIM binary. Unsure what that means.
	ReadTIM(tparam);	// Inits tparam as a TIM_IMAGE by reading the header data from tim. (I think?)
	
	LoadImage(tparam->prect, tparam->paddr);	// Load image data into VRAM.
	DrawSync(0);								// Wait for drawing to end. (Why?)
	
	if(tparam->mode & 0x8){							// Check if the TIM uses a CLUT (Color Look-up Table).
		LoadImage(tparam->crect, tparam->caddr);	// Load CLUT data into VRAM.
		DrawSync(0);
	}
}

void InitTextures(){
	LoadTexture(_binary_res_objs1_tim_start, timObjs1);
	LoadTexture(_binary_res_objs2_tim_start, timObjs2);
	LoadTexture(_binary_res_objs3_tim_start, timObjs3);
	LoadTexture(_binary_res_objs4_tim_start, timObjs4);
}

#endif