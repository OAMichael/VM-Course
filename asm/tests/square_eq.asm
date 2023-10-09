FMVI x1, 2.0f           ; a = 2.0f
FMVI x2, -5.0f          ; b = -5.0f
FMVI x3, 2.0f           ; c = 4.0f

FMUL x4, x1, x3         ; a*c
FMULI x4, x4, 4         ; 4*a*c
FMUL x5, x2, x2         ; b*b
FSUB x4, x5, x4         ; D = b*b - 4*a*c
FSQRT x4, x4            ; sqrt(D)
FMULI x6, x1, -2        ; -2 * a
FDIV x4, x4, x6         ; sqrt(D) / (-2 * a)

FDIV x5, x2, x6         ; b / (-2 * a)

FSUB x6, x5, x4         ; x_1

IMVI x7, 4012
STOREW x7, x6, 0
INTRINSIC x7, FPRINT    ; print x_1

FADD x6, x5, x4         ; x_2

IMVI x7, 4016
STOREW x7, x6, 0
INTRINSIC x7, FPRINT    ; print x_2

RET
