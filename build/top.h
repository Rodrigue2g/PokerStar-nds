
//{{BLOCK(top)

//======================================================================
//
//	top, 256x192@8, 
//	Transparent color : FF,FC,FF
//	+ palette 256 entries, not compressed
//	+ 410 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x24 
//	Total size: 512 + 26240 + 1536 = 28288
//
//	Time-stamp: 2024-01-05, 20:40:49
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TOP_H
#define GRIT_TOP_H

#define topTilesLen 26240
extern const unsigned int topTiles[6560];

#define topMapLen 1536
extern const unsigned short topMap[768];

#define topPalLen 512
extern const unsigned short topPal[256];

#endif // GRIT_TOP_H

//}}BLOCK(top)
