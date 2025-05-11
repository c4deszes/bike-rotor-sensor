Ride status requirements
========================

A ride is the act of the user getting on the bike, going somewhere, then getting off the bike.
In a system sense it's the state transition of the bike being idle then active then stopping at some
point.

This device is the primary responsible for indicating this state to others on the network as it has
the knowledge of when the bike is moving and when it's stationary.

.. req-sys:: Ride begin
    :id: REQ_SYS_RIDE_BEGIN
    :status: draft

    The ride begins once a speed above `5km/h` is detected on both wheels and the crank arm has been
    cycled once.

    The conditions only apply if these sensors operate normally, otherwise the particular condition
    is not necessary for the ride to begin. The crank revolution requirement is lifted if the speed
    is above ``10km/h``.

    Examples:

    - Front sensor failure: 5km/h must be detected at the rear, 1 crank arm revolution is needed
    - Crank sensor failure: 5km/h on both wheel sensors
    - All sensor failure: ride begins upon power up

.. req-sys:: Ride end
    :id: REQ_SYS_RIDE_END
    :status: draft

    The ride ends when the peripheral is requested to go into idle or shutdown modes.

.. req-sys:: Ride manual control
    :id: REQ_SYS_RIDE_CONTROL
    :status: draft

    The ride may begin or end at the user's request by sending a diagnostic command.
