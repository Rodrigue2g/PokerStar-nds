
//{{BLOCK(top)

//======================================================================
//
//	top, 256x192@8, 
//	Transparent color : FF,FC,FF
//	+ palette 256 entries, not compressed
//	+ 759 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x24 
//	Total size: 512 + 48576 + 1536 = 50624
//
//	Time-stamp: 2024-01-07, 18:14:54
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TOP_H
#define GRIT_TOP_H

#define topTilesLen 48576
extern const unsigned int topTiles[12144];

#define topMapLen 1536
extern const unsigned short topMap[768];

#define topPalLen 512
extern const unsigned short topPal[256];

#endif // GRIT_TOP_H

//}}BLOCK(top)
