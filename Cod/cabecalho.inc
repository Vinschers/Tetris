; by slmm 2020
;Assembler specific instructions for 32 bit ASM code

.386                   ; minimum processor needed for 32 bit
.model flat, stdcall   ; FLAT memory model & STDCALL calling
option casemap :none   ; set code to case sensitive

; este include tem que ser primeiro
include \masm32\include\windows.inc
include \masm32\include\user32.inc
include \masm32\include\kernel32.inc
include \masm32\include\gdi32.inc
include \masm32\include\masm32.inc
include \masm32\include\comdlg32.inc
include \masm32\include\shell32.inc
include \masm32\include\msimg32.inc
include \masm32\include\oleaut32.inc
include \masm32\macros\macros.asm
include \masm32\include\comctl32.inc
include \masm32\include\dnsapi.inc

; include das bibliotecas básicas do windows

includelib \masm32\lib\masm32.lib
includelib \masm32\lib\user32.lib
includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\gdi32.lib
includelib \masm32\lib\comctl32.lib
includelib \masm32\lib\comdlg32.lib
includelib \masm32\lib\shell32.lib
includelib \masm32\lib\msimg32.lib
includelib \masm32\lib\oleaut32.lib
includelib \masm32\lib\msvcrt.lib
includelib \masm32\lib\dnsapi.lib

include macros.inc