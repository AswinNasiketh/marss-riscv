/**
 * Simulation Trace Generator Utility
 *
 * MARSS-RISCV : Micro-Architectural System Simulator for RISC-V
 *
 * Copyright (c) 2020 Gaurav Kothari {gkothar1@binghamton.edu}
 * State University of New York at Binghamton
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <assert.h>
#include <stdlib.h>

#include "sim_trace.h"
#include "string.h"


void
sim_trace_start(SimTrace *s, const char *filename)
{
    s->trace_fp = fopen(filename, "w");
    assert(s->trace_fp);
}

void
sim_trace_stop(SimTrace *s)
{
    fclose(s->trace_fp);
}

void
sim_trace_commit(const SimTrace *s, uint64_t clock_cycle, int cpu_mode,
                 InstructionLatch *e, RISCVCPUState *cs)
{
    // fprintf(s->trace_fp, "cycle=%" TARGET_ULONG_FMT, clock_cycle);
    fprintf(s->trace_fp, "pc=%" TARGET_ULONG_HEX, e->ins.pc);
    // fprintf(s->trace_fp, " insn=%" PRIx32, e->ins.binary);
    fprintf(s->trace_fp, " %s", e->ins.str);

    //print source reg for compressed ISA conditional branches
    char cbeqz_str[] = "c.beqz";
    char cbnez_str[] = "c.bnez";
    if(strncmp(e->ins.str, cbeqz_str, 6) == 0 || 
        strncmp(e->ins.str, cbnez_str, 6) == 0 || 
    ){
        fprintf(s->trace_fp, " rs1_val=%" TARGET_ULONG_HEX, cs->reg[e->ins.rs1]);
    }

    char beq_str[] = "beq";
    char bne_str[] = "bne";
    char blt_str[] = "blt";
    char bge_str[] = "bge";
    char bltu_str[] = "bltu";
    char bgeu_str[] = "bgeu";


    //print source reg 1 and source reg 2 for conditional branches
    if(strncmp(e->ins.str, beq_str, 3) == 0 ||
        strncmp(e->ins.str, bne_str, 3) == 0 ||
        strncmp(e->ins.str, blt_str, 3) == 0 ||
        strncmp(e->ins.str, bge_str, 3) == 0 ||
        strncmp(e->ins.str, bltu_str, 4) == 0 ||
        strncmp(e->ins.str, bgeu_str, 4) == 0 
    ){
        fprintf(s->trace_fp, " rs1_val=%" TARGET_ULONG_HEX, cs->reg[e->ins.rs1]);
        fprintf(s->trace_fp, " rs2_val=%" TARGET_ULONG_HEX, cs->reg[e->ins.rs2]);
    }

    // fprintf(s->trace_fp, " mode=%s", cpu_mode_str[cpu_mode]); //disabled since all simulations will be done with user mode
    fprintf(s->trace_fp, "\n");
}

void
sim_trace_exception(const SimTrace *s, uint64_t clock_cycle, int cpu_mode,
                    SimException *e)
{
    fprintf(s->trace_fp, "cycle=%" TARGET_ULONG_FMT, clock_cycle);
    fprintf(s->trace_fp, " pc=%" TARGET_ULONG_HEX, e->pc);
    fprintf(s->trace_fp, " insn=%" PRIx32, e->insn);
    fprintf(s->trace_fp, " %s", e->insn_str);
    fprintf(s->trace_fp, " mode=%s", cpu_mode_str[cpu_mode]);
    fprintf(s->trace_fp, "\n");
}

SimTrace *
sim_trace_init()
{
    SimTrace *s;
    s = calloc(1, sizeof(SimTrace));
    assert(s);
    return s;
}

void
sim_trace_free(SimTrace **s)
{
    free(*s);
    *s = NULL;
}