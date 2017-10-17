char* vidmem = (char*)0xB8000;
const int VGA_WIDTH = 80;
const int VGA_HEIGHT = 25;
int cursor_x = 0;
int cursor_y = 0;

void set_cursor(int x, int y) {
	cursor_x = x;
	cursor_y = y;
}

void move_cursor(int dx, int dy) {
	cursor_x += dx;
	cursor_y += dy;
}

void put_char(char c, int color) {
	if (c == '\n') {
		set_cursor(0, cursor_y + 1);
		return;
	}

	const int index = (cursor_y * VGA_WIDTH + cursor_x) * 2;
	vidmem[index] = c;
	vidmem[index+1] = color;
	cursor_x ++;

	if (cursor_x >= VGA_WIDTH) {
		set_cursor(0, cursor_y + 1);
	}
}

void clear_screen(int color) {
	for (int y = 0; y < VGA_HEIGHT; y++) {
		for (int x = 0; x < VGA_WIDTH; x++) {
			const int index = (y * VGA_WIDTH + x) * 2;
			vidmem[index] = ' ';
			vidmem[index+1] = color;
		}
	}
	set_cursor(0, 0);
}

void draw_line_h(int x, int y, int length, char c, int color) {
	set_cursor(x, y);
	for (int cx = 0; cx < length; cx++) {
		put_char(c, color);
	}
}

void draw_line_v(int x, int y, int length, char c, int color) {
	set_cursor(x, y);
	for (int cx = 0; cx < length; cx++) {
		put_char(c, color);
		move_cursor(-1, 1);
	}
}

void draw_rect(int x, int y, int w, int h, int color) {
	draw_line_h(x, y, w, ' ', color);
	draw_line_h(x, y+h, w, ' ', color);
	draw_line_v(x, y+1, h-1, ' ', color);
	draw_line_v(x+w-1, y+1, h-1, ' ', color);
}

int strlen(const char *str)
{
    const char *s;
    for (s = str; *s; ++s);
    return(s - str);
}

void print(char* c, int color) {
	for (int i = 0; i < strlen(c); i++) {
		put_char(c[i], color);
	}
}

void kernel_main() {
	clear_screen(0x00);

	draw_rect(9, 7, 62, 8, 0x60);
	draw_rect(10, 7, 60, 8, 0x60);

	draw_rect(13, 9, 13, 4, 0x60);
	draw_rect(14, 9, 11, 4, 0x60);

	draw_rect(54, 9, 13, 4, 0x60);
	draw_rect(55, 9, 11, 4, 0x60);

	set_cursor(28, 10);
	print("   Welcome to PepeOS!   \n\n", 0x4f);
	move_cursor(28, 0);
	print(" Created by Pepe Becker \n", 0x30);

	set_cursor(1, VGA_HEIGHT - 2);
	print("root@pepeos -> \n", 0x0f);
}
