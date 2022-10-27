#include "qrand.h"

unsigned long long __qrand_state = 0x12345679;

unsigned int qrand()
{
    __qrand_state *= 0xFCF35B35;
    return __qrand_state>>32;
}