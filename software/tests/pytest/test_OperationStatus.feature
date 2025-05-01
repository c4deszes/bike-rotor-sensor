Feature: Operation status
    Tests whether the peripheral sets the operation status correctly

    Scenario: Ok operation
        Given base setup

        When I wait '500' milliseconds
        And I request operation status of 'RotorSensor' on 'LineMaster'

        Then the operation status of 'RotorSensor' shall be 'Ok'

    Scenario: Warn operation
        Given base setup

        When I change sensor 'FrontWheel' to 'Open'
        And I request operation status of 'RotorSensor' on 'LineMaster'

        Then the operation status of 'RotorSensor' shall be 'Warn'

    Scenario: Error operation
        Given base setup

        When I change sensor 'FrontWheel' to 'Open'
        When I change sensor 'RearWheel' to 'Open'
        And I request operation status of 'RotorSensor' on 'LineMaster'

        Then the operation status of 'RotorSensor' shall be 'Error'
