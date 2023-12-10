
//{{BLOCK(loading)

//======================================================================
//
//	loading, 256x192@8, 
//	+ palette 256 entries, not compressed
//	+ 768 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x24 
//	Total size: 512 + 49152 + 1536 = 51200
//
//	Time-stamp: 2023-12-10, 12:56:33
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LOADING_H
#define GRIT_LOADING_H

#define loadingTilesLen 49152
extern const unsigned int loadingTiles[12288];

#define loadingMapLen 1536
extern const unsigned short loadingMap[768];

#define loadingPalLen 512
extern const unsigned short loadingPal[256];

#endif // GRIT_LOADING_H

//}}BLOCK(loading)
