
//{{BLOCK(top)

//======================================================================
//
//	top, 256x192@8, 
//	Transparent color : FF,FF,FF
//	+ palette 256 entries, not compressed
//	+ 446 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x24 
//	Total size: 512 + 28544 + 1536 = 30592
//
//	Time-stamp: 2023-11-27, 21:23:36
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TOP_H
#define GRIT_TOP_H

#define topTilesLen 28544
extern const unsigned int topTiles[7136];

#define topMapLen 1536
extern const unsigned short topMap[768];

#define topPalLen 512
extern const unsigned short topPal[256];

#endif // GRIT_TOP_H

//}}BLOCK(top)
