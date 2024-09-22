%include    "../../emu_asm.s"

    global  _prog2_init
    global  _prog2_main

    section .text

_prog2_input:
    mov     edi,    NG_ASM_KEY_ESCAPE
    call    _ngGetKey
    test    eax,    3
    je      _prog2_input__exit
    xor     edi,    edi
    call    _asmEmuExit
_prog2_input__exit:
    ret

_prog2_main:
    call    _prog2_input
    mov     eax,    [rel redraw]    ; glfwTerminate() crash ako umesto EAX, stavim EBX
    test    eax,    1
    je      _prog2_main__afterdraw
    xor     edi,    edi
    xor     esi,    esi
    xor     edx,    edx
    call    _ngColor
    call    _ngClear

    mov     byte [rel redraw],  0
_prog2_main__afterdraw:
    ret

_prog2_init:
    mov     byte [rel redraw],  1
    ret


    section .bss
redraw:     resb    1
