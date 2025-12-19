#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Mon PID est : %d\n", getpid());

    volatile unsigned long long i = 0;
    
    while (1)
        i++;

    return 0;
}
