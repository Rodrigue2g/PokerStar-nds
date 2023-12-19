
//{{BLOCK(bottom)

//======================================================================
//
//	bottom, 256x192@8, 
//	+ palette 256 entries, not compressed
//	+ 769 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x24 
//	Total size: 512 + 49216 + 1536 = 51264
//
//	Time-stamp: 2023-12-18, 19:36:46
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BOTTOM_H
#define GRIT_BOTTOM_H

#define bottomTilesLen 49216
extern const unsigned int bottomTiles[12304];

#define bottomMapLen 1536
extern const unsigned short bottomMap[768];

#define bottomPalLen 512
extern const unsigned short bottomPal[256];

#endif // GRIT_BOTTOM_H

//}}BLOCK(bottom)
