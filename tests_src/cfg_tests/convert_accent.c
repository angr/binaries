#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

void frAccent2Eng(wchar_t* s)
{
    while (*s) {
        switch (*s) {
            case 130: //'é'
            case 136: //'ê'
            case 137: //'ë'
            case 138: //'è'
                *s = L'e';
                break;
            case 131: //'â'
            case 133: //'à'
                *s = L'a';
                break;
            case 129: //'ü'
            case 150: //'û'
            case 151: //'ù'
                *s = L'u';
                break;
            case 139: //'ï'
            case 140: //'î'
                *s = L'i';
                break;
            case 147: //'ô'
                *s = L'o';
                break;
            case 152: //'ÿ'
                *s = L'y';
                break;
            case 135: //'ç'
                *s = L'c';
                break;
            default:
                break;
        }
        s++;
    }
}

int main(int argc, char* argv[])
{
    wchar_t* s = wcsdup(L"abcc\x81\x82\x83\x85\x88\x93\x98\x8B\x8C"); 
    wprintf(L"%ls\n", s);
    frAccent2Eng(s);
    wprintf(L"\n%ls\n", s);
    free(s);
    return 0;
}
