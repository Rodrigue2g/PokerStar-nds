
//{{BLOCK(bkg)

//======================================================================
//
//	bkg, 256x192@8, 
//	+ palette 256 entries, not compressed
//	+ 769 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x24 
//	Total size: 512 + 49216 + 1536 = 51264
//
//	Time-stamp: 2024-01-03, 14:48:53
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BKG_H
#define GRIT_BKG_H

#define bkgTilesLen 49216
extern const unsigned int bkgTiles[12304];

#define bkgMapLen 1536
extern const unsigned short bkgMap[768];

#define bkgPalLen 512
extern const unsigned short bkgPal[256];

#endif // GRIT_BKG_H

//}}BLOCK(bkg)
