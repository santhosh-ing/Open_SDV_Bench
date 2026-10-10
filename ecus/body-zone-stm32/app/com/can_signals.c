#include "can_signals.h"

void pack_body_status(uint8_t *payload, bool hazard_req, uint8_t indicator_state, bool mirror_heat_req)
{
    (void)payload;
    (void)hazard_req;
    (void)indicator_state;
    (void)mirror_heat_req;
}

void unpack_body_status(const uint8_t *payload, bool *hazard_req, uint8_t *indicator_state, bool *mirror_heat_req)
{
    (void)payload;
    (void)hazard_req;
    (void)indicator_state;
    (void)mirror_heat_req;
}
