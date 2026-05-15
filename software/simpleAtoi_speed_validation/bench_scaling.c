#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int simpleAtoi(const char* s){
    int rslt = 0;
    int neg = 0;

    if (*s == '-'){ 
        neg = 1; s++;
    }

    while (*s >= '0' && *s <= '9'){
        rslt = rslt * 10 + (*s - '0');
        s++;
    }

    return neg ? -rslt : rslt;
}

static long bench(void (*fn)(volatile int*, const char*), volatile int* sink, const char* input){
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (int i = 0; i < 100000000; i++){
        fn(sink, input);
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);

    return (t1.tv_sec - t0.tv_sec) * 1000000000L + (t1.tv_nsec - t0.tv_nsec);
}

static void run_simple(volatile int* sink, const char* s){
    *sink += simpleAtoi(s);
}

static void run_stdlib(volatile int* sink, const char* s){ 
    *sink += atoi(s);
}

int main(void){
    volatile int sink = 0;
    const char* short_num = "42";
    const char* long_num  = "-2147483";

    long s_short = bench(run_simple, &sink, short_num);
    long s_long  = bench(run_simple, &sink, long_num);
    long a_short = bench(run_stdlib, &sink, short_num);
    long a_long  = bench(run_stdlib, &sink, long_num);

    printf("%-30s %ld ms\n", "simpleAtoi short  (\"42\"):",      s_short / 1000000);
    printf("%-30s %ld ms\n", "simpleAtoi long   (\"-2147483\"):", s_long  / 1000000);
    printf("%-30s %ld ms\n", "atoi       short  (\"42\"):",      a_short / 1000000);
    printf("%-30s %ld ms\n", "atoi       long   (\"-2147483\"):", a_long  / 1000000);
    printf("\nSpeedup short: %.1fx\n", (double)a_short / s_short);
    printf("Speedup long: %.1fx\n",  (double)a_long  / s_long);
    printf("(sink=%d)\n", (int)sink);

    return 0;
}
