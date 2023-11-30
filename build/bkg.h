
//{{BLOCK(bkg)

//======================================================================
//
//	bkg, 256x192@8, 
//	+ palette 256 entries, not compressed
//	+ 679 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x24 
//	Total size: 512 + 43456 + 1536 = 45504
//
//	Time-stamp: 2023-11-29, 12:01:50
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BKG_H
#define GRIT_BKG_H

#define bkgTilesLen 43456
extern const unsigned int bkgTiles[10864];

#define bkgMapLen 1536
extern const unsigned short bkgMap[768];

#define bkgPalLen 512
extern const unsigned short bkgPal[256];

#endif // GRIT_BKG_H

//}}BLOCK(bkg)
