#ifndef TEXTURES_H
#define TEXTURES_H

#include <libgpu.h>

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

#endif