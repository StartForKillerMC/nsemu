/* nsemu - LGPL - Copyright 2017 rkx1209<rkx1209dev@gmail.com> */
#include "Nsemu.hpp"
static Interpreter *cpu_engine;
namespace ARMv8 {

ARMv8State arm_state;

void Init() {
	Interpreter::create ();
	cpu_engine = Interpreter::get_instance ();
        cpu_engine->Init ();
        PC = 0x0;
        //PC = 0x30f0;
        SP = 0x3100000;
        SYSR.tpidrro_el[0] = (1 << 24) + 0x1000 * 1;
}

void RunLoop() {
	cpu_engine->Run ();
}

void Dump() {
        int cnt = 1;
        debug_print ("CPU Dump:\n");
        for (int r = 0; r < GPR_DUMMY; r++) {
                if (!X(r))
                        continue;
                if (r == GPR_LR)
                        debug_print ("LR:\t");
                else if (r == GPR_SP)
                        debug_print ("SP:\t");
                else
                        debug_print ("X%d:\t", r);
                debug_print ("0x%016lx%c", X(r), cnt % 3 == 0 ? '\n' : '\t');
                cnt++;
        }
        debug_print ("PC:\t0x%016lx\n", PC);
        debug_print ("NZCV:\t0x%016lx\n", NZCV);
}

static uint64_t counter;
void DumpJson(FILE *fp) {
        file_print (fp, "%lu : {\n", counter++);
        int r;
        for (r = 0; r < GPR_DUMMY; r++) {
                file_print (fp, "\"X%d\" : \"0x%016lx\",\n", r, X(r));
        }
        file_print (fp, "\"X%d\" : \"0x%016x\"\n", r, NZCV);
        file_print (fp, "},\n");
}

}
