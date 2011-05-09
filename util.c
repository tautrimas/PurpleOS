int max(int a, int b) {
    if (a > b) return a;
    else return b;
}

int min(int a, int b) {
    if (a < b) return a;
    else return b;
}

int strlen(char* str) {
    int i;
    for (i = 0; str[i] != 0; i++) {}
    return i;
}

void writeChar(int x, int y, char c) {
    unsigned char *videoram = (unsigned char *) 0xb8000;
    videoram[x * 2 + y * 160] = (unsigned char) c;
    videoram[x * 2 + y * 160 + 1] = 0x07;
}

void writeCharPos(int pos, char c) {
    writeChar(pos % 80, pos / 80, c);
}

void writeString(int x, int y, char* str) {
    int l = strlen(str);
    unsigned char *videoram = (unsigned char *) 0xb8000;
    int i;
    for (i = min(158 + 160*24, x * 2 + y * 160); i < l * 2; i+=2) {
	videoram[i] = str[i / 2];
    }
}

void clearScreen() {
    int i;
    for (i = 0; i < 160*25; i++)
	writeCharPos(i, ' ');
}
