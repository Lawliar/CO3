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


import subprocess
import os
#import nose
import avatar2 as avatar2

import angr
import claripy
from angr_targets import AvatarGDBConcreteTarget
import time

import threading

import sys
sys.path.append("../USBtest")

from serialEcho import send

from IPython import embed


# First set everything up

# Spawning of the gdbserver analysis environment

#import logging
#logging.getLogger('angr').setLevel('DEBUG')
#logging.getLogger('angr_targets.avatar_gdb').setLevel('DEBUG')
#logging.getLogger('avatar').setLevel('DEBUG')
#logging.getLogger("state_plugin").setLevel('DEBUG')
#logging.getLogger("pyvex").setLevel('DEBUG')

st_gdb_server = "/opt/st/stm32cubeide_1.14.0/plugins/com.st.stm32cube.ide.mcu.externaltools.stlink-gdb-server.linux64_2.1.100.202310302101/tools/bin/ST-LINK_gdbserver"
serial_number = "003F003D3532510131333430"
gdb_client = "/home/lcm/github/toolchains/arm/arm-gnu-toolchain-11.3.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-gdb"
GDB_SERVER_IP = "localhost"
GDB_SERVER_PORT = 61234

import symbionbytes

before_hal_addr = 0x8000a14
before_target_addr = 0x8000a2e
    
after_target_addr = 0x8000a33
sym_buff_addr = 0x2400041c

elf_file = "/home/lcm/github/spear/spear-code/firmware/STMfirmware/SymbionPLC/Debug/SymbionPLC.elf"

def runSymbion(concrete_input,output_dir = None):

    symbionbytes.numBytes = 0
    gdb_server = subprocess.Popen("{} -p {} -i {} -l 1 -d -s -cp /opt/st/stm32cubeide_1.14.0/plugins/com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.linux64_2.1.100.202311100844/tools/bin -m 0 -k".format(st_gdb_server,GDB_SERVER_PORT, serial_number),
    #                  stdout=subprocess.PIPE,
    #                  stderr=subprocess.PIPE,
                      shell=True
                       )

    time.sleep(2) ## to make sure the gdbserver is up
    # Instantiation of the AvatarGDBConcreteTarget
    
    start_time = time.time()
    avatar_gdb = AvatarGDBConcreteTarget(architecture=avatar2.archs.arm.ARM_CORTEX_M3,
                                         gdbserver_ip=GDB_SERVER_IP, 
                                         gdbserver_port=GDB_SERVER_PORT,
                                         gdb_executable=gdb_client)
    
    # Creation of the project with the new attributes 'concrete_target'
    p = angr.Project(elf_file, concrete_target=avatar_gdb,
                                 use_sim_procedures=True)
    
    entry_state = p.factory.entry_state()
    entry_state.options.add(angr.options.SYMBION_SYNC_CLE)
    entry_state.options.add(angr.options.SYMBION_KEEP_STUBS_ON_SYNC)
    
    simgr = p.factory.simgr(entry_state)

    ## Now, let's the binary unpack itself
    symbion = angr.exploration_techniques.Symbion(find=[before_hal_addr, ## before HAL_uart_receive
                                                        before_target_addr, ##  right before test
                                                        ])
    simgr.use_technique(symbion)

    simgr.run(thumb=True) 
    assert(len(simgr.found) == 1) ## make sure hit HAL_UART_RECEIVE
    simgr.move('found','active')


    t1 = threading.Thread(target = simgr.run,kwargs={'thumb':True})
    t2 = threading.Thread(target = send, args=(concrete_input,115200,"/dev/ttyACM0"))
    t1.start()
    time.sleep(0.3)##  have to wait for it to hit the break point to take place, then we feed the inputs, otherwise the input will be missed
    t2.start()
    t1.join()
    t2.join()

    
    assert(len(simgr.found) == 1) ## make sure hit before test

    concrete_state = simgr.stashes['found'][0]

    concrete_input_len =  concrete_state.solver.BVV(len(concrete_input),32)
    concrete_state.registers.store('r1',concrete_input_len)


    concrete_input_bvv = concrete_state.solver.BVV( concrete_input  ,len(concrete_input) * 8)
    concrete_state.memory.store(sym_buff_addr,concrete_input_bvv )
    concrete_state_copy = simgr.stashes['found'][0].copy()
    

    simgr.remove_technique(symbion)
    simgr.move('found','active')



    ## try to execute till the end of the target function
    ## this address is in thumb mode


    '''
    explore till the end of the target function
    '''


    trans_time = time.time() - start_time
    simgr.explore(find=after_target_addr,n = 99999999, thumb=True)

    trace = []


    for each_bb in simgr.found[0].history.bbl_addrs:
        trace.append(each_bb)
    trace.append(simgr.found[0].addr)

    ## spawn a new simgr based on the concrete state
    concrete_state_copy.options.add(angr.options.SYMBION_SYNC_CLE)
    concrete_state_copy.options.add(angr.options.SYMBION_KEEP_STUBS_ON_SYNC)
    tracer_simgr = p.factory.simgr(concrete_state_copy)
    ## setting the buffer
    sym_buffer = claripy.BVS('input_buffer',8*len(concrete_input))


    tracer_simgr.active[0].memory.store(sym_buff_addr,sym_buffer)

    ## use the simple tracer technique

    tracer = angr.exploration_techniques.SimpleTracer(trace)

    tracer_simgr.use_technique(tracer)
    tracer_simgr.use_technique(angr.exploration_techniques.Timeout(timeout=5)) ## same timeout with QEMU

    traced = tracer_simgr.run(n=len(trace),thumb=True)

    alternating_inputs = []
    for each_deviate_state in traced.deviated:
        alt_input = b''
        for i in range(concrete_input_bvv.size(), 0 ,-8):
            try:
                ## because, in angr, if a byte is unconstrainted, it's zero, but we want it to be the same with the input
                this_byte = each_deviate_state.solver.eval(sym_buffer[i-1:i-8], extra_constraints = [sym_buffer[i-1:i-8] == concrete_input_bvv[i-1 : i-8]])
            except angr.errors.SimUnsatError:
                this_byte = each_deviate_state.solver.eval(sym_buffer[i-1 : i-8])
            alt_input += this_byte.to_bytes(1,byteorder='little')
        alternating_inputs.append(alt_input)
    avatar_gdb.shutdown()

    total_time = time.time() - start_time
    time.sleep(1) ## waiting for it to really shutdown, otherwise, the subsequent connection will fail
    if output_dir == None:
        return alternating_inputs
    else:
        for id, each_input in enumerate(alternating_inputs):
            with open(os.path.join(output_dir, str(id)), 'wb') as wfile:
                wfile.write(each_input)
    return total_time, trans_time, symbionbytes.numBytes
if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("-i",required=True,help="input file")
    parser.add_argument("-o",required=True,help="output dir")
    args = parser.parse_args()
    with open(args.i,"rb") as wfile:
        concrete_input  = wfile.read()
    alt_inputs = runSymbion(concrete_input, args.o)
    
    
