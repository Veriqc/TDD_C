import numpy as np
from TDD import Index,get_tdd,get_identity_tdd,cont


class QuantumAutomata:
    def __init__(self,qubits=0,actions=[],operators={},initial_states=[]):
        self.qubits=qubits
        self.actions=actions
        self.operators=operators
        self.initial_states=initial_states

