#ifndef PADV_EXECMNG
#define PADV_EXECMNG
#include "PADV-NEXT/defines.h"

class ExecMng {
    public:
    static void Initialize();
    static void Reset(bool);
    static void Execute(ExecPhase);
    static void OnPhysicsUpdate(dAcPy_c* plr);
    static void* OnBcPickSource(void* out); //r3=out_data, r28=plr
};

ext void* LookupBc();
#endif