FMVI x1, 314.0f
FMVI x2, 228.0f

FSIN x4, x1             ; sin(x1)
FCOSI x5, 1488.0f       ; cos(1488)

FMUL x4, x2, x4         ; 228 * sin(x1)
FADD x4, x5, x4         ; cos(1488) + 228 * sin(x1)

IMVI x7, 4000
STOREW x7, x4, 0
INTRINSIC x7, FPRINT    ; print result
RET
