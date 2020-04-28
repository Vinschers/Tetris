; by slmm 2020
;Assembler specific instructions for 32 bit ASM code

.386                   ; minimum processor needed for 32 bit
.model flat, stdcall   ; FLAT memory model & STDCALL calling
option casemap :none   ; set code to case sensitive

; este include tem que ser primeiro
include \masm32\include\windows.inc

; include das bibliotecas básicas do windows
include \masm32\include\user32.inc
include \masm32\include\kernel32.inc
include \masm32\include\gdi32.inc

include \masm32\macros\macros.asm
include \masm32\include\masm32.inc
includelib \masm32\lib\masm32.lib

includelib \masm32\lib\user32.lib
includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\gdi32.lib

include macros.inc