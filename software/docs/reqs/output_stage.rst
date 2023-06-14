Output stage requirements
=========================

Startup
-------

.. req-sw:: Startup time
    :id: REQ_SW_OSH_STARTUP_TIME

    The output stage shall be turned on ``100ms`` after a Power-on-Reset, but no later than ``200ms``.

.. req-sw:: Startup error
    :id: REQ_SW_OSH_STARTUP_ERROR

    If either channel has an error in the initial ``500ms`` then the corresponding ``CHANNELx_ERROR``
    flags shall be immediately set to ``Short`` or ``Open`` depending on the fault condition.


Clearing & Retries
------------------

.. req-sw:: Startup retry
    :id: REQ_SW_OSH_STARTUP_RETRY

    The channels that encountered a startup error shall be retried ``800ms`` after Power-on in total
    5 times with ``200ms`` delay between retries.

    If the error is not clear for the last 3 attempts the channel shall be kept off until a Power-on-Reset.

.. req-sw:: Operational retry
    :id: REQ_SW_OSH_OPERATIONAL_RETRY

    The channels that encounter an error after the startup error window shall be retried for a total
    of 5 times with a minimum of ``1s`` delay between retries.

    If the error count reaches this maximum then the ``CHANNELx_ERROR`` flag shall be set to ``Short`` or ``Open``.

    If the error is cleared after an attempt then the ``CHANNELx_ERROR`` flag shall be set to ``Ok``.

