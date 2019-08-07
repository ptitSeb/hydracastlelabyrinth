#ifndef OPTIONS_H
#define OPTIONS_H

int options(int only);

int optionsStep();
void optionsDraw();
#ifdef _SDL
int getMusicType();
void setMusicType(int t);
#endif

#endif