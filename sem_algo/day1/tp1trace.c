////////////////////////////////////////////////////////////////
//                                                            //
//  DO NOT EDIT THIS FILE -- WRITE YOUR FUNCTIONS IN  tp1.c   //
//                                                            //
////////////////////////////////////////////////////////////////


// Reads the contents of "tp1.c", ignoring any main() function.
#define main no__main
#include "tp1.c"
#undef main

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

struct xx_ranctx { uint32_t a; uint32_t b; uint32_t c; uint32_t d; } xx_ranc;

uint32_t xx_ranval()
{
#define rot(x, k) (((x) << (k)) | ((x) >> (32 - (k))))
  uint32_t e = xx_ranc.a - rot(xx_ranc.b, 27);
  xx_ranc.a = xx_ranc.b ^ rot(xx_ranc.c, 17);
  xx_ranc.b = xx_ranc.c + xx_ranc.d;
  xx_ranc.c = xx_ranc.d + e;
  xx_ranc.d = e + xx_ranc.a;
  return xx_ranc.d;
}

void xx_raninit(uint32_t seed)
{
  xx_ranc.a = 0xf1ea5eed, xx_ranc.b = xx_ranc.c = xx_ranc.d = seed;
  for (uint32_t i = 0; i < 20; ++i)
    (void) xx_ranval();
}

FILE* xx_trace;
const char* xx_trace_name;

void xx_trace_init(const char* name)
{
  xx_trace_name = getenv("OUTPUT");
  if (!xx_trace_name)
    {
      static char tmp[30];
      strncpy(tmp, name, 23);
      strcat(tmp, ".trace");
      xx_trace_name = tmp;
    }
  xx_trace = fopen(xx_trace_name, "w+");
  if (!xx_trace)
    {
      fprintf(stderr, "cannot open '%s' for writing: %s\n",
	      xx_trace_name, strerror(errno));
      exit(2);
    }

  time_t rawtime = time(NULL);
  struct tm *timeinfo = gmtime(&rawtime);
  char buffer[20];
  strftime(buffer, 80, "%F %T%n", timeinfo);

  int u = getuid();
  int r = rawtime & 0xffffff;
  fprintf(xx_trace, "%s\n%d\n%d\n", name, u, r);
  fputs(buffer, xx_trace);
  u ^= r;
  const char* s = getenv("SEED");
  if (s)
    u = atoi(s);
  xx_raninit(u);
}

void xx_trace_done()
{
  fclose(xx_trace);
  printf("please submit '%s'\n", xx_trace_name);
}


unsigned int xx_cmp_count = 0;

int xx_cmp(int a, int b)
{
  ++xx_cmp_count;
  if (((a&1) == 0) && ((b&1) == 1))
    return -1;
  if (((a&1) == 1) && ((b&1) == 0))
    return 1;
  if (a < b)
    return -1;
  return a > b;
}

int main()
{
#if Q == 1
  xx_trace_init("tp1q1");
  for (int i = 10; i > 0; --i)
    {
      int r = (int) xx_ranval();
      fprintf(xx_trace, "int_width(%d) = %d\n", r, int_width(r));
    }
  int a[] =
    { -2147483647 - 1, -100000, -99999, -10000, -9999, -1000, -999, -100,
      -99, -10, -9, -1, 0, 1, 9, 10, 99, 100, 999, 1000, 9999, 10000,
      99999, 100000, 999999999, 1000000000,
      2147483647 };
  unsigned asize = sizeof(a) / sizeof(*a);
  for (unsigned i = 0; i < asize; ++i)
    fprintf(xx_trace, "int_width(%d) = %d\n", a[i], int_width(a[i]));
  xx_trace_done();
#elif Q == 2
  xx_trace_init("tp1q2");
  int a[11];
  for (int i = 0; i < 11; ++i)
    {
      int w = xx_ranval() % 31;
      if (i < 5 && w < 20)
	w += 10;
      a[i] = ((int) xx_ranval()) >> w;
    }
  fprintf(xx_trace, "a[] = { %d", a[0]);
  for (int i = 1; i < 11; ++i)
    fprintf(xx_trace, ", %d", a[i]);
  fprintf(xx_trace, " }\n");
  for (int i = 1; i < 11; ++i)
    fprintf(xx_trace, "ints_width(a, %d) == %d\n", i, ints_width(a, i));
  xx_trace_done();
#elif Q == 3
  xx_trace_init("tp1q3");
  int a[101];
  for (unsigned i = 0; i < 101; ++i)
    {
      int w = xx_ranval() % 31;
      a[i] = ((int) xx_ranval()) >> w;
    }
  print_int_array(xx_trace, a, 1);
  print_int_array(xx_trace, a, 4);
  print_int_array(xx_trace, a, 10);
  print_int_array(xx_trace, a, 20);
  print_int_array(xx_trace, a, 100);
  print_int_array(xx_trace, a, 101);
  for (unsigned i = 0; i < 40; ++i)
    a[i] = xx_ranval() % 10;
  print_int_array(xx_trace, a, 40);
  int tens = 1;
  for (unsigned s = 1; s <= 10; ++s)
    {
      tens *= 10;
      for (unsigned i = 0; i < 40 - 2 * s; ++i)
        a[i] = ((int) xx_ranval()) % tens;
      print_int_array(xx_trace, a, 40 - 2 * s);
    }
  xx_trace_done();
#elif Q == 5
  xx_trace_init("tp1q5");
  int a[100];
  for (unsigned i = 0; i < 100; ++i)
    {
      int w = xx_ranval() % 31;
      a[i] = ((int) xx_ranval()) >> w;
    }
  print_int_array(xx_trace, a, 100);
  insert_sort_cmp(a, 100, xx_cmp);
  fprintf(xx_trace, "%u comparisons\n", xx_cmp_count);
  print_int_array(xx_trace, a, 100);
  xx_trace_done();
#else
  puts("Compile with -DQ=n, where n is the question number.");
#endif
  return 0;
}
