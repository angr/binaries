// gcc calling_convention_0.c -O1 -m32 -c -fno-pie -fno-stack-protector -o ../tests/i386/calling_convention_0.o

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

struct a1 // sizeof=0x38
{
    const char char0;
    uint8_t gap1[33];
    int16_t int1622;
    uint8_t byte24;
    uint8_t byte25;
    uint8_t gap26[8];
    int16_t int162E;
    uint8_t gap30[4];
    int32_t dword34;
};

struct a4 // sizeof=0xB
{
    int16_t word0;
    int16_t word2;
    uint8_t byte4;
    uint8_t gap5;
    int16_t word6;
    int16_t word8;
    int8_t byteA;
};


int sub_8D9F0(const char *a1, const char *a2, int a3, char a4)
{
  const char *v4; // ecx
  const char *v5; // eax

  if ( a4 )
    _exit(0);
  v4 = a1;
  if ( !a1 )
    v4 = "<no reason given>";
  v5 = "halt";
  if ( !a4 )
    v5 = "warn";
  return fprintf(stderr, "EXCEPTION %s in %s,#%d: %s", v5, a2, a3, v4);
}

uint16_t * sub_119320(struct a1 *a1, int a2)
{
  int16_t *result; // eax
  int v3; // eax

  if ( !a1->byte24 )
  {
    sub_8D9F0("data->valid", "c:\\halo\\SOURCE\\memory\\data.c", 396, 1);
    _exit(-1);
  }
  if ( !a2 && a1->byte25 )
  {
    sub_8D9F0("identifier || !data->identifier_zero_invalid", "c:\\halo\\SOURCE\\memory\\data.c", 397, 1);
    _exit(-1);
  }
  if ( (a2 & 0x8000u) != 0
    || (int16_t)a2 >= a1->int162E
    || (result = (int16_t*)(a1->dword34 + (int16_t)a2 * a1->int1622), !*result)
    || a2 && a2 != *result )
  {
    sub_8D9F0("", "c:\\halo\\SOURCE\\memory\\data.c", 412, 1);
    _exit(-1);
  }
  return result;
}

void sub_8DB80(void*, int, int)
{
  // stub; do nothing
  return;
}

char sub_12000(int a1, int16_t a2, int16_t a3, struct a4 *a4)
{
  int16_t *v4; // edi
  int16_t v5; // ax

  v4 = sub_119320((struct a1 *)0x6325A4, a1);
  if ( !a4 )
  {
    sub_8D9F0("state_data", "c:\\halo\\SOURCE\\ai\\action_alert.c", 35, 1);
    _exit(-1);
  }
  sub_8DB80(a4, 0, 92);
  if ( *((uint8_t*)v4 + 6) )
    v5 = 0;
  else
    v5 = a2;
  a4->word0 = v5;
  a4->word6 = -1;
  a4->word8 = a3;
  a4->byte4 = 1;
  a4->word2 = 0;
  a4->byteA = 0;
  return 1;
}
