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

/*void writeChar(int x, int y, char c) {
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
}*/

void itoa(char *result, char base, int d) {
    char* digits = "0123456789abcdef";
    int size = 0;
    int copy = d;
    while (copy > 0) {
	copy /= base;
	size++;
    }
    size = max(size, 1);
    
    int i = 0;
    while (i < size) {
	char digit = digits[d % base];
	d /= base;
	result[size - i - 1] = digit;
	i++;
    }
    result[size] = 0;
}

/* Some screen stuff.  */
/* The number of columns.  */
#define COLUMNS			80
/* The number of lines.  */
#define LINES			24
/* The attribute of an character.  */
#define ATTRIBUTE		7
/* The video memory address.  */
#define VIDEO			0xB8000

/* Save the X position.  */
int xpos;
/* Save the Y position.  */
int ypos;
/* Point to the video memory.  */
static volatile unsigned char *video;

/* Clear the screen and initialize VIDEO, XPOS and YPOS.  */
void clearScreen (void)
{
  int i;

  video = (unsigned char *) VIDEO;
  
  for (i = 0; i < COLUMNS * LINES * 2; i++)
    *(video + i) = 0;

  xpos = 0;
  ypos = 0;
}

#define outb(value, port) __asm__ ( \
"outb    %%al,    %%dx\n\t"::"al"(value), "dx"(port))
 
void 
setCursor(int x, int y) {
    xpos = x;
    ypos = y;
    outb(0x0e, 0x3d4);
    outb(((xpos+ypos*COLUMNS)>>8)&0xff, 0x3d5);
    outb(0x0f, 0x3d4);
    outb(((xpos+ypos*COLUMNS))&0xff, 0x3d5);
}

/* Put the character C on the screen.  */
static void
putchar (int c)
{
  if (c == '\n' || c == '\r')
    {
    newline:
      xpos = 0;
      ypos++;
      if (ypos >= LINES)
	ypos = 0;
      return;
    }

  *(video + (xpos + ypos * COLUMNS) * 2) = c & 0xFF;
  *(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;

  xpos++;
  if (xpos >= COLUMNS)
    goto newline;
}

/* Format a string and print it on the screen, just like the libc
   function printf.  */
void
printf (const char *format, ...)
{
  char **arg = (char **) &format;
  int c;
  char buf[20];

  arg++;
  
  while ((c = *format++) != 0)
    {
      if (c != '%')
	putchar (c);
      else
	{
	  char *p;
	  
	  c = *format++;
	  switch (c)
	    {
	    case 'd':
	    case 'u':
	    case 'x':
	      itoa (buf, (c == 'x') ? 16 : 10, *((int *) arg++));
	      p = buf;
	      goto string;
	      break;

	    case 's':
	      p = *arg++;
	      if (! p)
		p = "(null)";

	    string:
	      while (*p)
		putchar (*p++);
	      break;

	    default:
	      putchar (*((int *) arg++));
	      break;
	    }
	}
    }
}
