Feature: Sensor control

    Scenario: Front ok
        Given base setup
        And I enable schedule 'RotorSensorDebug' on 'LineMaster'

        When I wait '500' milliseconds
        Then the signal 'SpeedSensorStatus.FrontSensorStatus' on 'LineMaster' shall be 'Ok'

    Scenario: Front short
        Given base setup
        And I enable schedule 'RotorSensorDebug' on 'LineMaster'

        When I change sensor 'FrontWheel' to 'Open'
        When I wait '500' milliseconds
        Then the signal 'SpeedSensorStatus.FrontSensorStatus' on 'LineMaster' shall be 'Ok'