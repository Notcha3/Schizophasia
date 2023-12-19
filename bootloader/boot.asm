[BITS 16]
[ORG 0x7c00]

MOV BH, 0x00 
_start:
MOV  AX, CS
MOV  DS, AX  
MOV CX, 20
CALL ClearScreen   
MOV SI, STRING1
CALL puts
CALL Sleep
MOV SI, STRING2
CALL puts
CALL Sleep
MOV SI, STRING3
CALL puts
CALL Sleep
MOV SI, STRING4
CALL puts
CALL Sleep
MOV CX, 5
CALL ClearScreen
CALL Sleep
MOV AL, 0
CALL RedScreen
JMP ManicEpisode

RedScreen:
MOV AX, 0600h
MOV BH, 11000111b
MOV CX, 0000h
MOV DX, 8025
INT 10h
MOV CX, 5
RET

ManicEpisode:
MOV SI, STRING6
CALL PutSmiley
CALL Sleep
CALL puts
JMP ManicEpisode

ClearScreen:
MOV AH, 0x00
MOV AL, 0x03
INT 0x10
RET

Sleep:
MOV AH, 86h
MOV DX, 2
INT 15h 
RET

puts:
        MOV AL, [SI]
        INC SI
        CMP AL, 0
        JE returnstart
        MOV AH, 0x0E
        INT 10h
        JMP puts
        returnstart:
        RET

PutSmiley:
        INC AL
        CMP AL, 3
        JE returnstart1
        MOV AH, 0x0E
        INT 10h
        JMP PutSmiley
        returnstart1:
        MOV AL, 0
        RET

STRING1 DB "I might be anywhere, lurking through the space.", 13, 10, 0
STRING2 DB "You will never see or hear me...", 13, 10, 0
STRING3 DB "Unless you look for me.", 13, 10, 0
STRING4 DB "/oO_Oo\", 13, 10, 0
STRING5 DB "Schizophasia", 0
STRING6 DB "WORTHLESS IDIOT", 0
TIMES 510 - ($ - $$) DB 0
DB 0x55
DB 0xAA