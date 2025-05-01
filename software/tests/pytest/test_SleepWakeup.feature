Feature: Sleep and Wakeup
    Tests whether the peripheral goes to sleep as requested and can wakeup

    Scenario: Wakeup (POR)
        Given off setup

        When I turn on the PSU
        And I wait '500' milliseconds
        And I request 'SpeedStatus' on 'LineMaster'

        #Then the response to 'SpeedStatus' should be valid

    Scenario: Sleep and wakeup
        Given off setup
        
        # Check after POR
        When I turn on the PSU
        And I wait '500' milliseconds
        And I request 'SpeedStatus' on 'LineMaster'
        #Then the response to 'SpeedStatus' should be valid

        When I send shutdown request on 'LineMaster'
        And I wait '500' milliseconds
        #Then the PSU current consumption shall be less than '500uA'

        When I send wakeup request on 'LineMaster'
        And I wait '500' milliseconds
        #Then the PSU current shall be more than '2mA'
        And I request 'SpeedStatus' on 'LineMaster'
