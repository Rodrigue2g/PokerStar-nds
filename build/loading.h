
//{{BLOCK(loading)

//======================================================================
//
//	loading, 256x192@8, 
//	+ palette 256 entries, not compressed
//	+ 736 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x24 
//	Total size: 512 + 47104 + 1536 = 49152
//
//	Time-stamp: 2024-01-08, 22:46:55
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LOADING_H
#define GRIT_LOADING_H

#define loadingTilesLen 47104
extern const unsigned int loadingTiles[11776];

#define loadingMapLen 1536
extern const unsigned short loadingMap[768];

#define loadingPalLen 512
extern const unsigned short loadingPal[256];

#endif // GRIT_LOADING_H

//}}BLOCK(loading)
