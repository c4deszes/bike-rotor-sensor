Feature: Bootloader
    Tests the most basic aspects of the bootloader integration

    Scenario: Boot entry
        Given base setup

        When I request operation status of 'RotorSensor' on 'LineMaster'
        Then the operation status of 'RotorSensor' shall be 'Ok'

        When I request boot entry of 'RotorSensor' on 'LineMaster'
        Then the boot response of 'RotorSensor' shall be "OpUnsafe'
        And the operation status of 'RotorSensor' shall be 'Boot'

    Scenario: Boot entry rejection
        Given base setup

        When I emulate speed of '20km/h'
        And I request boot entry of 'RotorSensor' on 'LineMaster'

        Then the boot response of 'RotorSensor' shall be "OpUnsafe'
        And the operation status of 'RotorSensor' shall be 'Ok'

