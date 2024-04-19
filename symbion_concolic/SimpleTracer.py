# This file is part of CO3.
#
# CO3 is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# CO3 is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# CO3. If not, see <https://www.gnu.org/licenses/>.

from typing import List, Dict, TYPE_CHECKING
import logging

from . import ExplorationTechnique
from .. import BP_BEFORE, BP_AFTER, sim_options
from ..errors import AngrTracerError, SimIRSBNoDecodeError

if TYPE_CHECKING:
    from angr.sim_state import SimState

from IPython import embed
l = logging.getLogger(name=__name__)




class SimpleTracer(ExplorationTechnique):

    def __init__(
        self,
        trace,
    ):
        super().__init__()
        self._trace = trace
        self.last_state = None

    def setup(self, simgr):
        simgr.populate("deviated", [])
        simgr.populate("traced", [])

        if len(simgr.active) != 1:
            raise AngrTracerError("Tracer is being invoked on a SimulationManager without exactly one active state")
        
        ## initialize the trace_idx into proper value
        active_addr = simgr.active[0].addr
        for idx, each_trace_addr in enumerate(self._trace):
            if(active_addr == each_trace_addr):
                simgr.active[0].globals["trace_idx"] = idx
                break
        if("trace_idx" not in simgr.active[0].globals):
            raise AngrTracerError("initial state's addr does not lie inside the trace")

    def complete(self, simgr):
        return bool(simgr.traced)

    def step(self, simgr, stash="active", **kwargs):
        return simgr.step(stash=stash, **kwargs)

    def step_state(self, simgr, state, **kwargs):
        if state.history.jumpkind == "Ijk_Exit":
            return {"traced": [state]}

        if state.globals["trace_idx"] == len(self._trace) - 1:
            return {"traced":[state]}
            

        expected_addr = self._trace[state.globals["trace_idx"] + 1]

        succs_dict = simgr.step_state(state,  **kwargs)
        if None not in succs_dict and simgr.errored:
            raise simgr.errored[-1].error
        sat_succs = succs_dict[None]  # satisfiable states
        #succs = sat_succs + succs_dict["unsat"]  # both satisfiable and unsatisfiable states

        found_next = False
        for each_succ in sat_succs:
            if each_succ.addr == expected_addr:
                ## this is the state that follows the trace
                ## it has to be sat
                each_succ.globals["trace_idx"] +=  1
                ##assert(each_succ in sat_succs)
                found_next = True
            else:
                ## this state deviate from the trace but it's sat
                if 'deviated' in succs_dict:
                    succs_dict['deviated'].append(each_succ)
                else:
                    succs_dict['deviated'] = [each_succ]
        if 'deviated' in succs_dict:
            succs_dict[None] = [x for x in succs_dict[None] if x not in succs_dict['deviated'] ]
        if len(succs_dict[None]) != 1 or found_next == False:
            print("sat state that follows the trace is not one")
            embed()
        
        return succs_dict


