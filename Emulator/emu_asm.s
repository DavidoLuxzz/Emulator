%define NG_ASM_KEY_ESCAPE 256
%define NG_ASM_KEY_W    87
%define NG_ASM_KEY_S    83
%define NG_ASM_KEY_UP   265
%define NG_ASM_KEY_DOWN 264
%define NG_ASM_KEY_G    71

; ngraphics variables
extern  ___ngScreenWidth
extern  ___ngScreenHeight
; ngraphics functions
extern  _ngGetKey
extern  _ngColor
extern  _ngClear
extern  _ngFontScale
extern  _ngDrawText
extern  _ngDrawRectangle
extern  _ngDrawLine
extern  _ngDrawTriangle
extern  _ngDrawSprite_GRAY
extern  _ngWait

; emu functions
extern  _asmEmuExit
extern  _asmEmuLog
