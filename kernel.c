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

void kmain( void* mbd, unsigned int magic )
{
    if ( magic != 0x2BADB002)
    {
	/* Something went not according to specs. Print an error */
	/* message and halt, but do *not* rely on the multiboot */
	/* data structure. */
    }
    
    // dummy stuff
    if ((int)mbd == 4684684) {}
    
    /* You could either use multiboot.h */
    /* (http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh) */
    /* or do your offsets yourself. The following is merely an example. */ 
    //char * boot_loader_name =(char*) ((long*)mbd)[16];

//     int i;
//     for (i = 0; i < 80; i++) {
//     }
    clearScreen();
    writeString(0, 0, "labas (:");

   /* Write your kernel here. */
}
