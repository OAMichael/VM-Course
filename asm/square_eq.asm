INTRINSIC x1, SCAN_F    ; a
INTRINSIC x2, SCAN_F    ; b
INTRINSIC x3, SCAN_F    ; c

MULF x4, x1, x3         ; a*c
MULIF x4, x4, 4         ; 4*a*c
MULF x5, x2, x2         ; b*b
SUBF x4, x5, x4         ; D = b*b - 4*a*c
SQRTF x4, x4            ; sqrt(D)
MULIF x6, x1, -2        ; -2 * a
DIVF x4, x4, x6         ; sqrt(D) / (-2 * a)

DIVF x5, x2, x6         ; b / (-2 * a)

SUBF x6, x5, x4         ; x_1
INTRINSIC x6, PRINT_F   ; print x_1

ADDF x6, x5, x4         ; x_2

INTRINSIC x6, PRINT_F   ; print x_2