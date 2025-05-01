Feature: PowerStatus
    Tests the diagnostic PowerStatus command

    Scenario: Voltage measurement (low)
        Given base setup
        And I enable the 'RotorSensorDebug' schedule
        
        When I set the supply voltage to '7V'
        And I wait '100ms'

        Then I should see that "RotorSensor.PowerStatus.Voltage" is equal to "7V"

    Scenario: Voltage measurement (normal)
        Given base setup
        And I enable the 'RotorSensorDebug' schedule
        
        When I set the supply voltage to '12V'
        And I wait '100ms'

        Then I should see that "RotorSensor.PowerStatus.Voltage" is equal to "12V"

    Scenario: Voltage measurement (high)
        Given base setup
        And I enable the 'RotorSensorDebug' schedule
        
        When I set the supply voltage to '16V'
        And I wait '100ms'

        Then I should see that "RotorSensor.PowerStatus.Voltage" is equal to "7V"
