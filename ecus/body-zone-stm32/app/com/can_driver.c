#include "can_driver.h"

void can_driver_init(void)
{
}

bool can_driver_send(const can_frame_t *frame)
{
    (void)frame;
    return true;
}

bool can_driver_receive(can_frame_t *frame)
{
    (void)frame;
    return false;
}
