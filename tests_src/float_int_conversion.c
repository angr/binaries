// for ARM Cortex-M:
// arm-none-eabi-gcc -mcpu=cortex-a7 -mthumb -ffunction-sections -Wl,--gc-sections -O0 -g -mfloat-abi=hard -mfpu=fpv5-d16 -o float_int_conversion.elf float_int_conversion.c

#include <stdio.h>

#define nop()  __asm__("nop")
void _exit(int a) { for(;;); }
int _sbrk() { return -1; }
int _close() { return -1; }
int _read() { return -1; }
int _write() { return -1; }
int _kill() { return -1; }
int _fstat() { return -1; }
int _isatty() { return -1; }
int _lseek() { return -1; }
int _getpid() { return -1; }

float current_angle;
float prev_angle;

int float_to_int(float f) {
  return (int)f;
}

float int_to_float(int i) {
  return (float)i;
}

float float_to_rounded_float(float f) {
  return (float)((int)f);
}

float increment_float(float f, float increment) {
  return f + increment;
}

int compare_floats(float a, float b, int delta) {
  if ((a - b) >= 0 && (a - b) < delta)
    return 1;
  else if ((b - a) >= 0 && (b - a) < delta)
    return 1;
  else
    return 0;
}

int main() {
  current_angle = 100.0;
  prev_angle = 200.0;

  int current_angle_int = float_to_int(current_angle);
  int prev_angle_int = float_to_int(prev_angle);
  printf("current_angle_int: %d\n", current_angle_int);
  printf("prev_angle_int: %d\n", prev_angle_int);

  float current_angle_2 = int_to_float(current_angle_int);
  float prev_angle_2 = int_to_float(prev_angle_int);
  printf("current_angle_int2: %f\n", current_angle_2);
  printf("prev_angle_int2: %f\n", prev_angle_2);

  current_angle = increment_float(current_angle, 10.0);
  prev_angle = increment_float(prev_angle, 8.0);
  printf("current_angle: %f\n", current_angle);
  printf("prev_angle: %f\n", prev_angle);

  float current_angle_rounded = float_to_rounded_float(current_angle);
  float prev_angle_rounded = float_to_rounded_float(prev_angle);
  printf("current_angle_rounded: %f\n", current_angle_rounded);
  printf("prev_angle_rounded: %f\n", prev_angle_rounded);

  if (compare_floats(current_angle, prev_angle, 30))
    printf("current_angle and prev_angle are within 30 degrees difference\n");
  else
    printf("current_angle and prev_angle are NOT within 30 degrees difference\n");

  return 0;
}
