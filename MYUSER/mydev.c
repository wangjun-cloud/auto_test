#include "mydev.h"

tMyDev TMyDev;

void mydev_init(tMyDev *p)
{
    uint8_t *s = (uint8_t *)p;
    uint32_t i;
    for (i = 0; i < sizeof(tMyDev); i++) s[i] = 0;
    p->soft_state   = SOFT_INIT;
    p->run_sub_state = RUN_SUB_IDLE;
}
