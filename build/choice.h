
//{{BLOCK(choice)

//======================================================================
//
//	choice, 256x192@8, 
//	+ palette 256 entries, not compressed
//	+ 383 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x24 
//	Total size: 512 + 24512 + 1536 = 26560
//
//	Time-stamp: 2024-01-08, 22:46:55
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_CHOICE_H
#define GRIT_CHOICE_H

#define choiceTilesLen 24512
extern const unsigned int choiceTiles[6128];

#define choiceMapLen 1536
extern const unsigned short choiceMap[768];

#define choicePalLen 512
extern const unsigned short choicePal[256];

#endif // GRIT_CHOICE_H

//}}BLOCK(choice)
