Feature: Line protocl
    Tests whether the peripheral sets the operation status correctly

    # Scenario: Wakeup, Idle, Shutdown
    #     Given base setup

    #     #When I request '' on 'LineMaster'
    #     When I send shutdown request on 'LineMaster'
    #     And I wait '500' milliseconds
    #     And I send wakeup request on 'LineMaster'
    #     And I wait '500' milliseconds
    #     And I send idle request on 'LineMaster'

    # Scenario: Request
    #     Given base setup

    #     #When I request 'SpeedStatus' on 'LineMaster'
    #     #And I wait '500' milliseconds
    #     When I set master signal 'LightSynchronization.TargetBrightness' to '100' on 'LineMaster'
    #     And I request 'LightSynchronization' on 'LineMaster'
    #     And I wait '1000' milliseconds
    #     When I set master signal 'LightSynchronization.TargetBrightness' to '10' on 'LineMaster'
    #     And I request 'LightSynchronization' on 'LineMaster'
    #     And I wait '1000' milliseconds
    #     When I set master signal 'LightSynchronization.TargetBrightness' to '50' on 'LineMaster'
    #     And I request 'LightSynchronization' on 'LineMaster'
    #     And I wait '1000' milliseconds

    Scenario: Schedule
        Given base setup

        When I enable schedule 'LightSchedule' on 'LineMaster'
        And I wait '500' milliseconds
        And I set master signal 'LightSynchronization.TargetBrightness' to '0' on 'LineMaster'
        And I wait '4' seconds
        And I set master signal 'LightSynchronization.TargetBrightness' to '10' on 'LineMaster'
        And I wait '4' seconds
        And I set master signal 'LightSynchronization.TargetBrightness' to '30' on 'LineMaster'
        And I wait '4' seconds
        And I set master signal 'LightSynchronization.TargetBrightness' to '50' on 'LineMaster'
        And I wait '4' seconds
        And I set master signal 'LightSynchronization.TargetBrightness' to '70' on 'LineMaster'
        And I wait '4' seconds
        And I set master signal 'LightSynchronization.TargetBrightness' to '100' on 'LineMaster'
        And I wait '4' seconds