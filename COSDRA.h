#ifndef MYLIB_H
#define MYLIB_H

//البمتغيرات والثوابت
volatile unsigned char *video_memory = (volatile unsigned char*)0xB8000;
int match_found = 0; 
char input_buffer[100];
int input_idx = 0;

// خريطة الكيبورد
unsigned char keyboard_map[128] = {
    0, 27,'1','2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',   0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0, '*',
    0,   ' '
};

//المقارنة
void compare_cmd(char *s1, char *s2) {
    int i = 0;
    match_found = 1; 
    while (s1[i] != '\0' || s2[i] != '\0') {
        if (s1[i] != s2[i]) {
            match_found = 0; 
            return; 
        }
        i++;
    }
}

//الإغلاق
void shuttdowen() {
    __asm__ volatile ("outw %0, %1" : : "a"((unsigned short)0x2000), "d"((unsigned short)0xB004));
    __asm__ volatile ("outw %0, %1" : : "a"((unsigned short)0x3400), "d"((unsigned short)0x4004));
}

//قراءة الكيبورد
unsigned char get_keyboard_input() {
    unsigned char result;
    __asm__ volatile("inb $0x60, %0" : "=a"(result));
    return result;
}

//مسح الشاشه
void clear_screen() {
    for (int i = 0; i < 80 * 25; i++) {
        video_memory[i * 2] = ' '; 
        video_memory[i * 2 + 1] = 0x07; 
    }
}

//طباعه النصوص
void print_str(char *name, unsigned char color, int x, int y) {
    for (int i = 0; name[i] != '\0'; i++) {
        int location = (y * 80 + (x + i)) * 2;
        video_memory[location] = name[i];
        video_memory[location + 1] = color;
    }
}

//تاخيل وقت ما
void dilay(int ts){
    for (int i = 0; i < ts; i++);
    
}

//طباعة حرف
void print_char(char c, unsigned char color, int x, int y) {
    // حسبة المكان المباشر في كارت الشاشة
    int location = (y * 80 + x) * 2;
    
    video_memory[location] = c;       // طباعة الحرف نفسه
    video_memory[location + 1] = color;  // وضع اللون
}

//طباعة الأرقام
void print_num(int num, unsigned char color, int x, int y) {
    int cursor_pos = (y * 80) + x;
    char buf[12]; 
    int i = 0;
    
    if (num == 0) { print_str("0", color, x, y); return; }
    
    while (num > 0) {
        buf[i++] = (num % 10) + '0';
        num /= 10;
    }
    for (int j = i - 1; j >= 0; j--) {
        video_memory[(cursor_pos) * 2] = buf[j];
        video_memory[(cursor_pos) * 2 + 1] = color;
        cursor_pos++;
    }
}

//© جميع الحقوق المحفوضه لعام 2026 Oshwad_dev ©️©️©️

#endif