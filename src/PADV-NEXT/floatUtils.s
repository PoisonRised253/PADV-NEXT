.text
.global fres__FRf
fres__FRf:
    lfs f1, 0(r3)
    fres f1, f1
    blr

.global fabs__FRf
fabs__FRf:
    lfs f1, 0(r3)
    fabs f1, f1
    blr

.global fnabs__FRf
fnabs__FRf:
    lfs f1, 0(r3)
    fnabs f1, f1
    blr

.global getLR__Fv
getLR__Fv:
    mflr r3
    blr

.global getTB__Fv
getTB__Fv:
    mftbu r3
    mftb r4
    blr
