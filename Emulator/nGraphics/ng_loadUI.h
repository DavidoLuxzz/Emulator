//
//  ng_loadUI.h
//  Emulator
//
//  Created by Luka on 17.9.24..
//

#ifndef ng_loadUI_h
#define ng_loadUI_h

struct NG_LOADER_ENTRY {
    int id;
    const char* name;
};
typedef struct NG_LOADER_ENTRY ng_lde;

void ngUISetup(unsigned int w, unsigned int h, unsigned int maxprog);
void ngUIShow(void);
void ngUIAddEntry(int id, const char* sname);
void ngUISelectNext(void);
void ngUISelectPrevious(void);
unsigned int ngUIGetSelectedEntry(void);

void ngUIDestroy(void);

#endif /* ng_loadUI_h */
