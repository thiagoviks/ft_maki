#include <ft_maki.h>
#include <ft_math.h>
#include <time.h>

int main(void) {
  // ft_printf("ints: %d %ld %lld\n", 123, 123L, 123LL);
  // ft_printf("unsigned: %u %lu\n", 4294967295u, 4294967295ul);
  // ft_printf("hex: %x %X\n", 3735928559u, 3735928559u);
  // ft_printf("ptr: %p\n", (void*)0x12345);
  // ft_printf("float: %.0f %.2f %.6f\n", 9.9995, 9.9995, 3.141592653589793);
  // ft_printf("nan/inf: %f %f %f\n", FT_NAN, FT_INFINITY, -FT_INFINITY);
  // ft_printf("percent: %%\n");

  // clock_t start, end;
  clock_t start2, end2;

  // start = clock();
  // for (int i = 0; i < 100000; ++i)
  //   printf("Number: %d\n", i);
  // end = clock();

  start2 = clock();
  for (int i = 0; i < 100000; ++i)
    ft_printf("Number: %d\n", i);
  end2 = clock();
  // printf("printf took %.2fs\n", (double)(end - start) / CLOCKS_PER_SEC);
  ft_printf("ft_printf took %.2fs\n", (double)(end2 - start2) / CLOCKS_PER_SEC);

  return (0);
}