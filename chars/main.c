#include <stdio.h>

typedef struct { char v[5]; } Emoji;

int main(void){
    Emoji emoji_char = { .v = "🤣" };
    printf("Size of emoji_char: %ld\n", sizeof(emoji_char));
    printf("Size of char: %ld\n", sizeof(char));
    printf("%s\n", (char*)emoji_char.v);
    return 0;
}
