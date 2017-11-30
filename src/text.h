#ifndef TEXT_HEADER
#define TEXT_HEADER

#define JAPANESE 0
#define ENGLISH 1

char gameLanguage;

typedef struct {
	unsigned char x[32];
	unsigned char y[32];
	char length;
} Message;

Message* saving;
Message* saveError[3];
Message* itemName[41];
Message* found;
Message* itemDescription[28];
Message* dungeon[8];

void textInit();
void textFree();

void loadText();

int drawText(Message* m, int x, int y);
int drawCharacter(int cx, int cy, int x, int y);
void drawTextCentered(Message* m, int x, int y);

void setLanguage(char lan);
char getLanguage();

#endif