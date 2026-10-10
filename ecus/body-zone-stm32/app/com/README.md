# com – CAN communication

Wraps the bxCAN HAL driver and implements the `interfaces/can/body.dbc` contract: frame validation, alive counters, cycle times and timeout supervision.

`generated/` holds the pack/unpack code produced by `tools/gen_can.sh`. Never edit it by hand.
