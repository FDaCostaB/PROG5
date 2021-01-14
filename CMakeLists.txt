cmake_minimum_required(VERSION 3.17)
project(PROG5 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(PROG5
        arm.c
        arm.h
        arm_branch_other.c
        arm_branch_other.h
        arm_constants.c
        arm_constants.h
        arm_core.c
        arm_core.h
        arm_data_processing.c
        arm_data_processing.h
        arm_exception.c
        arm_exception.h
        arm_instruction.c
        arm_instruction.h
        arm_load_store.c
        arm_load_store.h
        arm_simulator.c
        config.h
        csapp.c
        csapp.h
        debug.c
        debug.h
        gdb_protocol.c
        gdb_protocol.h
        memory.c
        memory.h
        memory_test.c
        no_trace_location.h
        registers.c
        registers.h
        scanner.c
        scanner.h
        send_irq.c
        trace.c
        trace.h
        trace_location.h
        util.c
        util.h)
