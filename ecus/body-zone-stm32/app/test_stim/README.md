# test_stim – loopback test stimulus

Plays the central computer while no CAN transceiver is wired: sends `BODY_LightCmd` through the normal CAN driver in silent loopback mode; the user button steps through test scenarios. Compiled only when `BODY_CAN_LOOPBACK` is set (spec 002).
