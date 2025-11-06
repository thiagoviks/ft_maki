#include <ft_maki.h>

/*
** ============================================================================
** TEST 1: Basic Lock/Unlock
** ============================================================================
*/
void test_basic_lock_unlock(void) {
  ft_mutex_t mutex;

  ft_printf("\n=== Test 1: Basic Lock/Unlock ===\n");

  // Initialize
  if (ft_mutex_init(&mutex) != 0) {
    ft_printf("❌ Failed to initialize mutex\n");
    return;
  }
  ft_printf("✅ Mutex initialized (state=%d)\n", mutex.futex_word);

  // Lock
  if (ft_mutex_lock(&mutex) != 0) {
    ft_printf("❌ Failed to lock mutex\n");
    return;
  }
  ft_printf("✅ Mutex locked (state=%d)\n", mutex.futex_word);

  // Unlock
  if (ft_mutex_unlock(&mutex) != 0) {
    ft_printf("❌ Failed to unlock mutex\n");
    return;
  }
  ft_printf("✅ Mutex unlocked (state=%d)\n", mutex.futex_word);

  // Destroy
  if (ft_mutex_destroy(&mutex) != 0) {
    ft_printf("❌ Failed to destroy mutex\n");
    return;
  }
  ft_printf("✅ Mutex destroyed\n");
}

/*
** ============================================================================
** TEST 2: Trylock
** ============================================================================
*/
void test_trylock(void) {
  ft_mutex_t mutex;

  ft_printf("\n=== Test 2: Trylock ===\n");

  ft_mutex_init(&mutex);

  // First trylock should succeed
  if (ft_mutex_trylock(&mutex) == 0) {
    ft_printf("✅ Trylock succeeded on unlocked mutex\n");
  } else {
    ft_printf("❌ Trylock failed on unlocked mutex\n");
    return;
  }

  // Second trylock should fail (already locked)
  if (ft_mutex_trylock(&mutex) != 0) {
    ft_printf("✅ Trylock correctly failed on locked mutex\n");
  } else {
    ft_printf("❌ Trylock incorrectly succeeded on locked mutex\n");
  }

  // Unlock
  ft_mutex_unlock(&mutex);

  // Trylock should succeed again
  if (ft_mutex_trylock(&mutex) == 0) {
    ft_printf("✅ Trylock succeeded after unlock\n");
  } else {
    ft_printf("❌ Trylock failed after unlock\n");
  }

  ft_mutex_unlock(&mutex);
  ft_mutex_destroy(&mutex);
}

/*
** ============================================================================
** TEST 3: Multiple Lock/Unlock Cycles
** ============================================================================
*/
void test_multiple_cycles(void) {
  ft_mutex_t mutex;
  int i;

  ft_printf("\n=== Test 3: Multiple Lock/Unlock Cycles ===\n");

  ft_mutex_init(&mutex);

  for (i = 0; i < 1000; i++) {
    ft_mutex_lock(&mutex);
    ft_mutex_unlock(&mutex);
  }

  ft_printf("✅ 1000 lock/unlock cycles completed\n");
  ft_mutex_destroy(&mutex);
}

/*
** ============================================================================
** TEST 4: Protected Counter (Simulated Race)
** ============================================================================
*/

static ft_mutex_t g_counter_mutex;
static int g_counter = 0;

void increment_counter_unsafe(void) {
  int temp = g_counter;
  // Simulate some work
  for (int i = 0; i < 100; i++)
    ;
  g_counter = temp + 1;
}

void increment_counter_safe(void) {
  ft_mutex_lock(&g_counter_mutex);
  int temp = g_counter;
  // Simulate some work
  for (int i = 0; i < 100; i++)
    ;
  g_counter = temp + 1;
  ft_mutex_unlock(&g_counter_mutex);
}

void test_protected_counter(void) {
  ft_printf("\n=== Test 4: Protected Counter ===\n");

  ft_mutex_init(&g_counter_mutex);

  // Reset counter
  g_counter = 0;

  // Single-threaded test (should work)
  for (int i = 0; i < 100; i++) {
    increment_counter_safe();
  }

  ft_printf("✅ Counter value after 100 increments: %d\n", g_counter);

  if (g_counter == 100) {
    ft_printf("✅ Counter is correct!\n");
  } else {
    ft_printf("❌ Counter is incorrect (expected 100)\n");
  }

  ft_mutex_destroy(&g_counter_mutex);
}

/*
** ============================================================================
** TEST 5: Stress Test - Rapid Lock/Unlock
** ============================================================================
*/
void test_stress(void) {
  ft_mutex_t mutex;
  int iterations = 100000;
  int i;

  ft_printf("\n=== Test 5: Stress Test (%d iterations) ===\n", iterations);

  ft_mutex_init(&mutex);

  t_timeval start, end;
  ft_gettimeofday(&start, FT_NULL);

  for (i = 0; i < iterations; i++) {
    ft_mutex_lock(&mutex);
    // Critical section (empty for benchmark)
    ft_mutex_unlock(&mutex);
  }

  ft_gettimeofday(&end, FT_NULL);

  long elapsed_us = (end.ft_tv_sec - start.ft_tv_sec) * 1000000 +
                    (end.ft_tv_usec - start.ft_tv_usec);

  ft_printf("✅ Completed %d lock/unlock cycles\n", iterations);
  ft_printf("✅ Time: %ld microseconds\n", elapsed_us);
  ft_printf("✅ Average per operation: %ld nanoseconds\n",
            (elapsed_us * 1000) / iterations);

  ft_mutex_destroy(&mutex);
}

/*
** ============================================================================
** TEST 6: Nested Lock Detection (Should Deadlock)
** ============================================================================
*/
void test_nested_lock_warning(void) {
  ft_printf("\n=== Test 6: Nested Lock (Will Hang - Ctrl+C to Skip) ===\n");
  ft_printf("NOTE: Recursive locking is NOT supported.\n");
  ft_printf("Skipping this test to avoid hanging...\n");

  /*
  ** Uncomment to demonstrate deadlock:
  **
  ** ft_mutex_t mutex;
  ** ft_mutex_init(&mutex);
  ** ft_mutex_lock(&mutex);
  ** ft_printf("First lock acquired\n");
  ** ft_mutex_lock(&mutex);  // This will deadlock!
  ** ft_printf("This will never print\n");
  */
}

int main(int argc, char **argv) {

  ft_printf("\n");
  ft_printf("╔════════════════════════════════════════════════════════╗\n");
  ft_printf("║        FT_MAKI FUTEX MUTEX TEST SUITE                  ║\n");
  ft_printf("╚════════════════════════════════════════════════════════╝\n");

  test_basic_lock_unlock();
  test_trylock();
  test_multiple_cycles();
  test_protected_counter();
  test_stress();
  test_nested_lock_warning();

  ft_printf("\n");
  ft_printf("╔════════════════════════════════════════════════════════╗\n");
  ft_printf("║                 ALL TESTS COMPLETED                    ║\n");
  ft_printf("╚════════════════════════════════════════════════════════╝\n");
  ft_printf("\n");

  if (argc > 1) {
    // Declaramos um ponteiro(link para o endereço da memória) para o
    // arquivo de nome: 'pf'
    T_FT_FILE *pf;
    char conteudo[100];

    // Abre o arquivo novamente para leitura
    pf = ft_fopen(argv[1], "r");

    // Le em conteudo o valor da variável armazenada anteriormente pf
    ft_fread(&conteudo, sizeof(char), 100, pf);

    // Imprime o conteúdo, se existir, do arquivo informado
    ft_printf("\nO CONTEÚDO DO ARQUIVO É:\n%s \n", conteudo);

    ft_fclose(pf);
  } else {
    ft_printf("Informe o arquivo. Ex.: ./program arquivo.txt\n");
  }

  return (0);
}
