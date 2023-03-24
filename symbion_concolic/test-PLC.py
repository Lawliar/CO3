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

import logging
logging.getLogger('angr').setLevel('DEBUG')
logging.getLogger('angr_targets.avatar_gdb').setLevel('DEBUG')
logging.getLogger('avatar').setLevel('DEBUG')
logging.getLogger("state_plugin").setLevel('DEBUG')
#pyvex.lifting.libvex
logging.getLogger("pyvex").setLevel('DEBUG')
st_gdb_server = "/home/lcm/st/stm32cubeide_1.10.1/plugins/com.st.stm32cube.ide.mcu.externaltools.stlink-gdb-server.linux64_2.0.300.202203231527/tools/bin/ST-LINK_gdbserver"

gdb_client = "/home/lcm/github/toolchains/arm/asan-0x24000000/bin/arm-none-eabi-gdb"
GDB_SERVER_IP = "localhost"
GDB_SERVER_PORT = 61234





def main(elf_file,before_hal_addr,before_target_addr,concrete_input,after_target_addr,sym_buff_addr):
    gdb_server = subprocess.Popen("{} -p {} -l 1 -d -s -cp /home/lcm/st/stm32cubeide_1.10.1/plugins/com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.linux64_2.0.301.202207041506/tools/bin -m 0 -k".format(st_gdb_server,GDB_SERVER_PORT),
    #                  stdout=subprocess.PIPE,
    #                  stderr=subprocess.PIPE,
                      shell=True
                       )

    time.sleep(2) ## to make sure the gdbserver is up
    # Instantiation of the AvatarGDBConcreteTarget
    print("Creating avatar target..")
    avatar_gdb = AvatarGDBConcreteTarget(architecture=avatar2.archs.arm.ARM_CORTEX_M3,
                                         gdbserver_ip=GDB_SERVER_IP, 
                                         gdbserver_port=GDB_SERVER_PORT,
                                         gdb_executable=gdb_client)
    print("Loading binary into angr..")
    # Creation of the project with the new attributes 'concrete_target'
    p = angr.Project(elf_file, concrete_target=avatar_gdb,
                                 use_sim_procedures=True)
    print("creating entry state..")
    entry_state = p.factory.entry_state()
    entry_state.options.add(angr.options.SYMBION_SYNC_CLE)
    entry_state.options.add(angr.options.SYMBION_KEEP_STUBS_ON_SYNC)
    print("creating simgr..")
    simgr = p.factory.simgr(entry_state)

    ## Now, let's the binary unpack itself
    symbion = angr.exploration_techniques.Symbion(find=[before_hal_addr, ## before HAL_uart_receive
                                                        before_target_addr, ##  right before test
                                                        ])
    simgr.use_technique(symbion)

    simgr.run(thumb=True) 
    assert(len(simgr.found) == 1) ## make sure hit HAL_UART_RECEIVE
    simgr.move('found','active')

    print("HAL_uart_receive is hit")

    t1 = threading.Thread(target = simgr.run,kwargs={'thumb':True})
    t2 = threading.Thread(target = send, args=(concrete_input,115200,"/dev/ttyACM0"))
    t1.start()
    time.sleep(2)##  have to wait for it to hit the break point to take place, then we feed the inputs, otherwise the input will be missed
    t2.start()
    t1.join()
    t2.join()

    assert(len(simgr.found) == 1) ## make sure hit before test

    concrete_state = simgr.stashes['found'][0]

    concrete_input_len =  concrete_state.solver.BVV(len(concrete_input),32)
    concrete_state.registers.store('r1',concrete_input_len)

    concrete_state_copy = simgr.stashes['found'][0].copy()
    print("before test is hit")

    simgr.remove_technique(symbion)
    simgr.move('found','active')



    ## try to execute till the end of the target function
    ## this address is in thumb mode


    '''
    explore till the end of the target function
    '''
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

    traced = tracer_simgr.run(thumb=True)

    alternating_inputs = []
    for each_deviate_state in traced.deviated:
        alternating_inputs.append(each_deviate_state.solver.eval(sym_buffer))
    avatar_gdb.shutdown()
    time.sleep(1) ## waiting for it to really shutdown, otherwise, the subsequent connection will fail
    return alternating_inputs
if __name__ == '__main__':
    elf_file = "/home/lcm/github/spear/spear-code/firmware/STMfirmware/SymbionPLC/Debug/SymbionPLC.elf"
    before_hal_addr = 0x80010b4
    before_target_addr = 0x80010ce
    concrete_input = b"2131231dasdasdasdsa231"
    after_target_addr = 0x80010d3
    sym_buff_addr = 0x24000a7c
    alt_inputs = main(elf_file, before_hal_addr, before_target_addr, concrete_input, after_target_addr, sym_buff_addr)
