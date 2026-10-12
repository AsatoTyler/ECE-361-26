#include <stdio.h>
#include <stdlib.h>
#include "reading.h"
#include "stats.h"
#include "histogram.h"

#define MAX_READINGS 1000

int main(int argc, char *argv[]) {
    // Non global variables
    int ticks[MAX_READINGS];
    float temps[MAX_READINGS];
    float hums[MAX_READINGS];
    int count = 0;
    int skipped = 0;
    float threshold = 30.0f;

    // Parse command line args
    if (argc > 2) {
        fprintf(stderr, "usage: %s [threshold] < readings.txt\n", argv[0]);
        return 1;
    }
    if (argc == 2) {
        char *end;
        threshold = strtof(argv[1], &end);
        if (*end != '\0') {
            fprintf(stderr, "error: threshold '%s' is not a number\n", argv[1]);
            return 1;
        }
    }

    // Read inputted data
    reading_read(ticks, temps, hums, MAX_READINGS, &count, &skipped);

    /* Print basic counts */
    printf("readings: %d\n", count);
    printf("skipped:  %d\n", skipped);
    if (count == 0) {
        printf("no readings, no summary\n");
        return 0;
    }

    // Temp
    float tmin = stats_min(temps, count);
    float tmax = stats_max(temps, count);
    float tmean = stats_mean(temps, count);
    printf("temperature: min %6.1f  max %6.1f  mean %6.2f C\n", tmin, tmax, tmean);

    // Humidity
    float hmin = stats_min(hums, count);
    float hmax = stats_max(hums, count);
    float hmean = stats_mean(hums, count);
    printf("humidity:    min %6.1f  max %6.1f  mean %6.2f %%RH\n", hmin, hmax, hmean);

    // Longest run of readings above the threshold
    int best_start;
    int best_len = stats_longest_run_above(temps, count, threshold, &best_start);
    
    if (best_len == 0) {
        printf("above %.1f C: never\n", threshold);
    } else {
        printf("above %.1f C: longest run %d readings, from tick %d to tick %d\n",
               threshold, best_len, ticks[best_start], ticks[best_start + best_len - 1]);
    }

    // Histogram of temps
    histogram_print(temps, count);

    return 0;
}