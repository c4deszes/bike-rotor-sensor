import pytest
from pytest_bdd import scenarios, given, when, then, parsers
from bike_testframework.bdd.common import *
from bike_testframework.bdd.line_steps import generate_line_steps
from line_protocol.protocol.new_master import LineMaster
from line_protocol.protocol import LineSerialTransport
from line_protocol.network import Network, load_network

@pytest.fixture
def network():
    return load_network("../customer/network.json")

@pytest.fixture
def line_master(network):
    with LineSerialTransport('COM14', network.baudrate, one_wire=True) as phy:
        with LineMaster(phy, network) as master:
            yield master

@given(parsers.parse("base setup"))
def base_setup():
    pass

@given(parsers.parse("off setup"))
def off_setup():
    pass

@when(parsers.parse("I turn on the PSU"))
def turn_on_psu():
    pass

@when(parsers.parse("I turn off the PSU"))
def turn_off_psu():
    pass

generate_line_steps("line_master", "LineMaster")
