#include <stdio.h>
#include <time.h>

int simpleAtoi(const char* s){
    int rslt = 0;
    int neg = 0;

    if(*s == '-'){ 
        neg = 1; s++; 
    }

    while(*s >= '0' && *s <= '9'){
        rslt = rslt * 10 + (*s - '0');
        s++;
    }
    
    return neg ? -rslt : rslt;
}

int main(void){
    const char* tests[] = { "42", "-7", "1234567", "0", "-999999" };
    volatile int sink = 0;
    struct timespec t0, t1;

    clock_gettime(CLOCK_MONOTONIC, &t0);
    
    for(int i = 0; i < 100000000; i++){
        sink += simpleAtoi(tests[i % 5]);
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);

    long ns = (t1.tv_sec - t0.tv_sec) * 1000000000L + (t1.tv_nsec - t0.tv_nsec);
    printf("simpleAtoi: %ld ms (sink=%d)\n", ns / 1000000, (int)sink);

    return 0;
}
