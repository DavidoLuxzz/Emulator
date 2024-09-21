; // func(rdi, rsi, rdx, rcx, ...)
; // return [rax]

%include    "../emu_asm.s"

global _prog1_main
global _prog1_init

; TEST instrukcija:
;  test eax, 1  -  ZF = !(eax & 1)

    section .text
_prog1_input:
_prog1_input__escape:
    mov     edi,    NG_ASM_KEY_ESCAPE   ; key = escape
    call    _ngGetKey
    cmp     eax,    1
    jl      _prog1_input__w             ; if false, go to next if
    mov     edi,    0                   ; true:
    call    _asmEmuExit
_prog1_input__w:
    mov     edi,    NG_ASM_KEY_W        ; key = w
    call    _ngGetKey
    test    eax,    3
    je      _prog1_input__s
    mov     edx,    [rel pyp]           ; true:
    mov     ecx,    [rel pv]
    sub     edx,    ecx
    mov     dword [rel pyp],    edx
_prog1_input__s:
    mov     edi,    NG_ASM_KEY_S        ; key = s
    call    _ngGetKey
    test    eax,    3
    je      _prog1_input__up
    mov     edx,    [rel pyp]           ; true:
    mov     ecx,    [rel pv]
    add     edx,    ecx
    mov     dword [rel pyp],    edx
_prog1_input__up:
    mov     edi,    NG_ASM_KEY_UP       ; key = up
    call    _ngGetKey
    test    eax,    3
    je      _prog1_input__down
    mov     edx,    [rel pyp+4]         ; true:
    mov     ecx,    [rel pv]
    sub     edx,    ecx
    mov     dword [rel pyp+4],    edx
_prog1_input__down:
    mov     edi,    NG_ASM_KEY_DOWN     ; key = down
    call    _ngGetKey
    test    eax,    3
    je      _prog1_input__g
    mov     edx,    [rel pyp+4]         ; true:
    mov     ecx,    [rel pv]
    add     edx,    ecx
    mov     dword [rel pyp+4],    edx
_prog1_input__g:                        ; debug print scores
    mov     edi,    NG_ASM_KEY_G        ; key = g
    call    _ngGetKey
    test    eax,    3
    je      _prog1_input__exit
    mov     edi,    [rel scores]
    call    _asmEmuLog
    mov     edi,    [rel scores+4]
    call    _asmEmuLog
_prog1_input__exit:                     ; end input
    ret

_prog1_fix_pp:  ; fix player y position
    ; if (p1y-PHL < 0):
    mov     edx,    [rel pyp]
    cmp     edx,    40
    jnl     _prog1_fix_pp__if2
    mov     dword [rel pyp],    40
_prog1_fix_pp__if2:
    ; if (p1y+40 > __ngScreenHeight)
    add     edx,    40
    mov     ecx,    [rel ___ngScreenHeight]
    cmp     edx,    ecx
    jng     _prog1_fix_pp__if3
    sub     ecx,    40
    mov     dword [rel pyp],    ecx
_prog1_fix_pp__if3:
    ; if (p2y-PHL < 0):
    mov     edx,    [rel pyp+4]
    cmp     edx,    40
    jnl     _prog1_fix_pp__if4
    mov     dword [rel pyp+4],    40
_prog1_fix_pp__if4:
    ; if (p1y+40 > __ngScreenHeight)
    add     edx,    40
    mov     ecx,    [rel ___ngScreenHeight]
    cmp     edx,    ecx
    jng     _prog1_fix_pp__exit
    sub     ecx,    40
    mov     dword [rel pyp+4],    ecx
_prog1_fix_pp__exit:
    ret

_prog1_pip:     ; edi = (int) playerID
    test    edi,    1
    jne     _prog1_pip__p2   ; edi != 1
    ; bally+10 >= (p1y-PADDLE_HALF_LENGTH) && bally-10 <= (p1y+PADDLE_HALF_LENGTH)
    mov     edx,    [rel bally]
    add     edx,    10
    mov     ecx,    [rel pyp]
    sub     ecx,    40 ; PHL = 40
    cmp     edx,    ecx
    jl      _prog1_pip__exit0
    ; drugi deo ifa
    sub     edx,    20
    add     ecx,    80
    cmp     edx,    ecx
    jg      _prog1_pip__exit0
    mov     eax,    1
    ret
_prog1_pip__p2:
    mov     edx,    [rel bally]
    add     edx,    10
    mov     ecx,    [rel pyp+4]
    sub     ecx,    40
    cmp     edx,    ecx
    jl      _prog1_pip__exit0
    ; drugi deo ifa
    sub     edx,    20
    add     ecx,    80
    cmp     edx,    ecx
    jg      _prog1_pip__exit0
    mov     eax,    1
    ret
_prog1_pip__exit0:
    xor     eax,    eax
    ret

_prog1_score_up:    ; edi = (int) playerID
    ; scores[playerID] += 1
    lea     rsi,    [rel scores]
    mov     rax,    4           ; sizeof(int)
    mul     edi                 ; eax = edi*sizeof(int)
    add     rsi,    rax
    mov     ecx,    [rsi]
    add     ecx,    1
    mov     dword [rsi],    ecx
    
    ; sscores[playerID] = (scores[playerID] + '0')  // scores[playerID] sacuvano u ECX
    lea     rsi,    [rel sscores+2]
    mov     rax,    2
    mul     edi
    sub     rsi,    rax
    mov     cl,     [rsi]
    add     cl,    1
    mov     byte [rsi],     cl

    mov     eax,    [rel ___ngScreenWidth]
    xor     edx,    edx ; da ne bi bio error (nez)
    mov     ecx,    2
    div     ecx
    mov     dword [rel ballx],  eax
    mov     eax,    [rel ___ngScreenHeight]
    xor     edx,    edx
    div     ecx
    mov     dword [rel bally],  eax

    mov     rdi,    60
    call    _ngWait
    ret

_prog1_move_ball:
    ; ballx += balldx
    mov     edi,    [rel ballx]
    mov     edx,    [rel balldx]
    add     edi,    edx
    mov     [rel ballx],    edi
    ; bally += balldy
    mov     edi,    [rel bally]
    mov     edx,    [rel balldy]
    add     edi,    edx
    mov     [rel bally],    edi
    
    ; if (ballx-10 <= 15):
    mov     edi,    [rel ballx]
    sub     edi,    10
    cmp     edi,    15
    jg      _prog1_move_ball__if0exit
    ; if (prog1_pip(0)):
    xor     edi,    edi
    call    _prog1_pip
    test    eax,    1
    je      _prog1_move_ball__score_up0
    ; else: balldx = (0) -balldx
    mov     edx,    [rel balldx]
    xor     ecx,    ecx
    sub     ecx,    edx
    mov     dword [rel balldx], ecx
    jmp     _prog1_move_ball__if0exit
_prog1_move_ball__score_up0:
    ; prog1_score_up(0)
    xor     edi,    edi
    call    _prog1_score_up
_prog1_move_ball__if0exit:    
    ; if (ballx+25 >= __ngScreenWidth):
    mov     edi,    [rel ballx]
    add     edi,    25
    mov     edx,    [rel ___ngScreenWidth]
    cmp     edi,    edx
    jl      _prog1_move_ball__if1exit
    ; if (prog1_pip(1))
    mov     edi,    1
    call    _prog1_pip
    test    eax,    1
    je      _prog1_move_ball__score_up1
    ; balldx = (0) -balldx
    mov     edx,    [rel balldx]
    xor     ecx,    ecx
    sub     ecx,    edx
    mov     dword [rel balldx], ecx
    jmp     _prog1_move_ball__if1exit
_prog1_move_ball__score_up1:
    ; prog1_score_up(1)
    mov     edi,    1
    call    _prog1_score_up
_prog1_move_ball__if1exit:
    ; if (bally-13 <= 0 || bally+13 >= __ngScreenHeight):
    mov     edi,    [rel bally]
    sub     edi,    13
    xor     edx,    edx
    cmp     edi,    edx
    jle      _prog1_move_ball__negdy
    ; drugi deo ifa --  (bally+13 >= __ngScreenHeight)
    add     edi,    26
    mov     edx,    [rel ___ngScreenHeight]
    cmp     edi,    edx
    jl      _prog1_move_ball__exit
_prog1_move_ball__negdy:
    ; balldy = (0) -balldy
    mov     edi,    [rel balldy]
    xor     ecx,    ecx
    sub     ecx,    edi
    mov     dword [rel balldy], ecx
    ; end if
_prog1_move_ball__exit:
    ret

_prog1_main:
    call    _prog1_input
    call    _prog1_fix_pp
    ; if (redraw):
    mov     ax,     [rel redraw]
    ; cmp je pravio probleme pa sam video da GCC koristi TEST instrukciju koja je benger
    test    ax,     1               ; ZF = ((redraw AND 1)!=0)
    je      _prog1_main__exit       ; ZF == 1
    
    ; clear screen
    xor     edi,    edi
    xor     esi,    esi
    xor     edx,    edx
    call    _ngColor
    call    _ngClear
    ; set color for drawing scores
    mov     edi,    50
    mov     esi,    50
    mov     edx,    50
    call    _ngColor
    ; draw scores
    mov     edi,    200
    mov     esi,    100
    lea     rdx,    [rel sscores]
    mov     ecx,    3
    call    _ngDrawText
    ; reset color back to NG_LAZY_WHITE (200,200,200)
    mov     edi,    200
    mov     esi,    200
    mov     edx,    200
    call    _ngColor
    ; draw ball ngDrawSprite_GRAY(x,y, sprite, sdata)
    mov     edi,    [rel ballx]
    sub     edi,    10
    mov     esi,    [rel bally]
    sub     esi,    10
    lea     rdx,    [rel BALL_BYTES]
    lea     rcx,    [rel BALL_DATA]
    call    _ngDrawSprite_GRAY
    ; draw paddle 1
    mov     edi,    5               ; ngDrawRectangle(5, p1y-PHL, 10, 2*PHL)
    mov     esi,    [rel pyp]
    sub     esi,    40              ; p1y - 40  // PHL = const 40
    mov     edx,    10
    mov     ecx,    80              ; 2*PHL = 2*40 = 80
    call    _ngDrawRectangle
    ; draw paddle 1
    mov     edi,    [rel ___ngScreenWidth] ; ngDrawRectangle(src_width-15, p2y-40, 10, 80)
    sub     edi,    15
    mov     esi,    [rel pyp+4]
    sub     esi,    40
    mov     edx,    10
    mov     ecx,    80
    call    _ngDrawRectangle
_prog1_main__exit:
    ; move ball as a last function
    call    _prog1_move_ball
    ret


_prog1_init:
    mov     edi,    24
    call    _ngFontScale
    mov     byte  [rel redraw],  1       ; redraw = 1
    mov     rdx,    0
    mov     ecx,    2
    mov     eax,    [rel ___ngScreenWidth]
    div     ecx
    mov     dword [rel ballx],  eax     ; ballx = __ngScreenWidth/2
    mov     rdx,    0
    mov     eax,    [rel ___ngScreenHeight]
    div     ecx
    mov     dword [rel bally],  eax     ; bally = __ngScreenHeight/2
    mov     dword [rel balldx], 4       ; balldx = 4
    mov     dword [rel balldy], 4       ; balldy = 4
    mov     dword [rel pyp],    eax     ; p1y = __ngScreenHeight/2
    mov     dword [rel pyp+4],  eax     ; p2y = __ngScreenHeight/2
    mov     dword [rel pv],     6       ; pv = 6
    mov     dword [rel scores],     0   ; scores[0] = 0
    mov     dword [rel scores+4],   0   ; scores[1] = 0
    mov     byte [rel sscores],     '0' ; sscores[0] = '0'
    mov     byte [rel sscores+1],   ':' ; sscores[1] = ':'
    mov     byte [rel sscores+2],   '0' ; sscores[2] = '0'
    ret

; constants
    section .data
BALL_DATA:          ; ball data (x,y, color_mul, px_size, transparent)
    dd  7,7, 200,3, 1
BALL_BYTES:         ; ball sprite (8x8)*2
    db  0,0,1,1,1,0,0
    db  0,1,1,1,1,1,0
    db  1,1,1,1,1,1,1
    db  1,1,1,1,1,1,1
    db  1,1,1,1,1,1,1
    db  0,1,1,1,1,1,0
    db  0,0,1,1,1,0,0

; variables
    section .bss
redraw:     resb    1
ballx:      resd    1
bally:      resd    1
balldx:     resd    1
balldy:     resd    1
pyp:        resd    2
pv:         resd    1
scores:     resd    2
sscores:    resb    3
