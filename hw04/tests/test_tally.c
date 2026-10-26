#include "../tally.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

static int passes = 0;
static int failures = 0;

static void test_assert(bool condition, const char *name) {
    if (condition) {
        printf("PASS: %s\n", name);
        passes++;
    } else {
        printf("FAIL: %s\n", name);
        failures++;
    }
}

int main(void) {
    /* 1. Counting up & 2. A name never added */
    tally_t *t1 = tally_create();
    test_assert(t1 != NULL, "create first tally");
    test_assert(tally_add(t1, "apple") == 1, "counting up (first add)");
    test_assert(tally_add(t1, "apple") == 2, "counting up (second add)");
    test_assert(tally_count(t1, "apple") == 2, "count reflects additions");
    test_assert(tally_count(t1, "banana") == 0, "a name never added returns 0");

    /* 3. An empty name */
    test_assert(tally_add(t1, "") == -1, "empty name refused");

    /* 4. A name of exactly TALLY_LEN characters and one of TALLY_LEN + 1 */
    char exact_len[TALLY_LEN + 1];
    memset(exact_len, 'x', TALLY_LEN);
    exact_len[TALLY_LEN] = '\0';
    
    char too_long[TALLY_LEN + 2];
    memset(too_long, 'y', TALLY_LEN + 1);
    too_long[TALLY_LEN + 1] = '\0';

    test_assert(tally_add(t1, exact_len) == 1, "name of exactly TALLY_LEN accepted");
    test_assert(tally_add(t1, too_long) == -1, "name of TALLY_LEN + 1 refused");

    /* 5. NULL arguments */
    test_assert(tally_add(NULL, "test") == -1, "NULL tally for add refused");
    test_assert(tally_add(t1, NULL) == -1, "NULL name for add refused");
    test_assert(tally_count(NULL, "test") == 0, "NULL tally for count returns 0");
    test_assert(tally_size(NULL) == 0, "NULL tally for size returns 0");

    /* 6. A full tally */
    tally_t *t_full = tally_create();
    char buf[32];
    for (int i = 0; i < TALLY_NAMES; i++) {
        sprintf(buf, "item%d", i);
        tally_add(t_full, buf);
    }
    test_assert(tally_add(t_full, "overflow") == -1, "new name refused when tally is full");
    test_assert(tally_add(t_full, "item0") == 2, "existing name still counts when tally is full");

    /* 7. TALLY_MAX tallies, then one more (NULL) */
    tally_t *pool_test[TALLY_MAX];
    pool_test[0] = t1;
    pool_test[1] = t_full;
    for (int i = 2; i < TALLY_MAX; i++) {
        pool_test[i] = tally_create();
    }
    tally_t *extra = tally_create();
    test_assert(extra == NULL, "create one more than TALLY_MAX returns NULL");

    /* 8. Create after destroy */
    tally_destroy(pool_test[TALLY_MAX - 1]);
    pool_test[TALLY_MAX - 1] = tally_create();
    test_assert(pool_test[TALLY_MAX - 1] != NULL, "create after destroy succeeds");

    /* 9. Two tallies are independent */
    tally_add(pool_test[2], "unique");
    test_assert(tally_count(pool_test[2], "unique") == 1, "first independent tally counts unique");
    test_assert(tally_count(pool_test[3], "unique") == 0, "second independent tally remains unaffected");

    /* 10. The print order */
    tally_destroy(t1);  /* Free a slot in the pool before creating a new one */
    tally_t *t_order = tally_create();
    
    tally_add(t_order, "zebra");
    tally_add(t_order, "aardvark");
    tally_add(t_order, "monkey");

    FILE *tmp = tmpfile();
    tally_print(t_order, tmp);
    rewind(tmp);
    
    /* Initialize arrays to prevent garbage memory if read fails */
    char line1[32] = {0}, line2[32] = {0}, line3[32] = {0};
    
    if (fgets(line1, sizeof(line1), tmp) &&
        fgets(line2, sizeof(line2), tmp) &&
        fgets(line3, sizeof(line3), tmp)) {
        
        test_assert(strcmp(line1, "aardvark 1\n") == 0 && 
                    strcmp(line2, "monkey 1\n") == 0 && 
                    strcmp(line3, "zebra 1\n") == 0, 
                    "print order is alphabetical");
    } else {
        test_assert(false, "print order is alphabetical (file read failed)");
    }
    
    fclose(tmp);
    
    /* Clean up the remaining tallies before exiting */
    tally_destroy(t_full);
    tally_destroy(pool_test[2]);
    tally_destroy(pool_test[3]);
    tally_destroy(t_order);

    printf("\nTests passed: %d\nTests failed: %d\n", passes, failures);
    return failures > 0 ? 1 : 0;
}