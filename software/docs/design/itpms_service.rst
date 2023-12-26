iTPMS service
=============

Responsibilities are:

* Monitoring wheel speeds and determining tire pressure differences

State machine
-------------

During startup the iTPMS service is started automatically, the actual measurement start is delayed
by ``2 seconds``.

After this any errors or unreliable speed reports will cause the iTPMS service to stop and will
require a manual restart.

If no errors occur then after ``5 minutes`` of measurement the service makes a conclusion based on the
accumulated difference of wheel speeds. The tire with the higher indicated speed is considered to be
lower pressure.

Questions
---------

* Could we make it so it automatically restarts if the channel speeds are stable? E.g.: monitor
  channel state, if it's okay for 1 min then start it.

* The current concept doesn't tell the user if both tires are low pressure. Is there any reference
  we could use that's accurate enough for such a measurement? E.g.: total GPS distance over 10km was
  less than what they should have been given the wheel rotations, or monitor vibrations?
