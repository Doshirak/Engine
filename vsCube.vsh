vs_1_1

dcl_position v0
dcl_color0 v4

; c0 = matWorldViewProjT
; c4 = alpha, 1.0 - alpha, 0, 0

def c5, 0.5, 0.5, 0.5, 1.0 


; ************* v0 = 0.5 * v0 / sqrt(v0) *************
; r1 = v0 * v0
dp3 r1, v0, v0 

; r1 = 1 / sqrt(v0 * v0)
rsq r2, r1

; r0 = 0.5 * v0 / sqrt(v0)
mul r0, v0, r2
mul r0, r0, c5	
; r0.w = 1.0
mov r0.w, c5.w
; ****************************************************

; r1 = v0 * alpha
mul r1, v0, c4.x
mov r1.w, c5.w

; r2 = r0 * (1.0 - alpha)
mul r2, r0, c4.y
mov r2.w, c5.w

; r0 = r1 + r2
add r0, r1, r2


m4x4 oPos, r0, c0
mov oD0, v4