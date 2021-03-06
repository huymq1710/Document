
NUM_0 EQU 40h ; code to display 0 on 7 segment
NUM_1 EQU 79h ; code to display 1 on 7 segment
NUM_2 EQU 24h ; code to display 2 on 7 segment
NUM_3 EQU 30h ; code to display 3 on 7 segment
NUM_4 EQU 19h ; code to display 4 on 7 segment
NUM_5 EQU 12h ; code to display 0 on 7 segment
NUM_6 EQU 02h ; code to display 1 on 7 segment
NUM_7 EQU 78h ; code to display 2 on 7 segment
NUM_8 EQU 00h ; code to display 3 on 7 segment
NUM_9 EQU 10h ; code to display 4 on 7 segment
;Variable
LED_DATA EQU 0x40
;Interrupt table
ORG 0000h 		; entry address for 8051 RESET
	LJMP MAIN 	; move MAIN away from interrupt vector table
ORG 0003h 		; vector address for interrupt 0
	LJMP ISR0 	; jump to ISR0
ORG 0013h 		; vector address for interrupt 1
	LJMP ISR1 	; jump to ISR1
;=================================================================
;Code segment
;=================================================================
ORG 0100h 				; MAIN starts here
MAIN:
    MOV P1,#0ffh
	MOV P2,#0ffh
	MOV IE, #10000101B 	; enable external interrupts IE0, IE1
	SETB IT0 			; negative edge trigger for interrupt 0
	SETB IT1 			; negative edge trigger for interrupt 1
	MOV R0, #2			;Trang thai htai d�n, bd R0=0
	MOV 40h, #NUM_0		;dat o dia chi 40h o trong RAM : dua gtri NUM_0 vao dia chi 40 
	MOV 41h, #NUM_1				; NUM_x c� gi� tri o bang gi� tri line 6->15
	MOV 42h, #NUM_2
	MOV 43h, #NUM_3
	MOV 44h, #NUM_4
	MOV 45h, #NUM_5
	MOV 46h, #NUM_6
	MOV 47h, #NUM_7
	MOV 48h, #NUM_8
	MOV 49h, #NUM_9
;Code 43-56: TH s� <0 hay >9 thi ko thuc hien nua, kh� v� 8051 ko co lenh so s�nh ( 8086 c� l�nh so s�nh CMP)
LOOP:
    MOV A, P1				;ktra CANCEL 
	CJNE A, #00h, CANCEL

	MOV A, P2				;Ktra OK
	CJNE A, #00h, ALARM

	MOV A, R0			; move data to A
						; Ktr A=R0 c� < 0 
						; (vi d�y la s� ko d�u => 0 - 1 =255 ) => Ktr bit cao nh�t =1 hay =0
	RLC A			 	; Lenh quay tr�i to�n b� bit cua A, bit cao nh�t cua A v�o C
	JNC BIGGER10		; JNC : Nhay neu co Carry C=0
						;Neu C=0 => bit cao nhat cua A=0 => A>0	: Kiem tra tiep	o nhan BIGGER10 TH A>10
						;Neu C=1 => A<0 => JNC ko thuc hien, xuong duoi hien thi 0
CANCEL:	MOV R0, #0		; Khi d� A<0, set no tro lai ve 0  v� xu�ng duoi hien thi	
	JMP DISPLAY
BIGGER10:
	MOV A, R0			; move data to A
	CLR C
	SUBB A, #10			; A-10	( phai clear C=0 o line 61 )	 
	JC DISPLAY			; neu A<10 khi tru ben line 59 c� muon co C	=> C=1 => JC nhay xuong lenh day ma ko thuc hien n�
	MOV R0, #9		
DISPLAY:	 			 ;Code duoi, Tu R0: hien thi dlieu ra ngoai , sd lookup table b�n tr�n 34-43
			   			;=> chi can lay gtri o dia chi (R0+40h) la biet de hien s� n�o
	MOV A, R0		
	ORL A, #40h			; A= R0+40h	  ; ORL lenh OR vi gtri A:0-9
	MOV R1, A			;R1=dia chi so can hien
	MOV P0, @R1	  		;P0=gtr dia chi o R1 de hien
	CALL DELAY_1MS
	LJMP LOOP 			; end loop 

ALARM:
		
  TRU:CALL DIS			;Code dem giam dan gtri tren den LED ve 0
      CALL DELAY_1S
	  DEC R0
      MOV A,R0
      CJNE A, #0, TRU
   CALL DIS
   CJNE A, #9, COI	   ;Tai gtri 0, bat C�I
	   
   LJMP LOOP

;============================================================
; ONE_MILLI_SUB
; Subroutine to delay ONE millisecond
; Uses and preserves R7
;============================================================
DELAY_1MS:
	PUSH 07h 				; save R7 to stack
	MOV R7, #250d 			; 
LOOP_1_MILLI: 				; loops 250 times
	NOP 					; two NOPs
	NOP 					;
	DJNZ R7, LOOP_1_MILLI 	; loop until zero
	POP 07h 				; restore R7 
	RET 					; return to caller



DELAY_50MS: 
	CLR ET0
	ANL TMOD, #0F0h
	ORL TMOD, #01h
	MOV TH0, #3Ch
	MOV TL0, #0B0h
	CLR TF0
	SETB TR0
  WAIT_50MS:
    JNB TF0, WAIT_50MS
	RET 					; return to caller

DELAY_1S:					; 1s =20x50ms, su dung vong lap 20 lan
	MOV R2,#20
	BACK: CALL DELAY_50MS
		  DJNZ R2, BACK	
	RET
DIS:	 		
	MOV A, R0		
	ORL A, #40h		
	MOV R1, A		
	MOV P0, @R1
	CALL DELAY_1MS
	RET

COI: 
   SETB P0.7
   CALL DELAY_1S
   LJMP LOOP
;==================================================================
; ISR0 						Thanh Ghi R0 nhiem vu luu trang th�i hien tai bi�n d�m
;==================================================================
ISR0:			  ;chuc nang tang bien dem
	INC  R0					; Increase data
	RETI 					; return from interrupt
;==================================================================
; ISR1 
;==================================================================
ISR1:			  ;chuc nang giam bien dem 
	DEC R0					; decrease value
	RETI  	 					; return from interrupt
;==================================================================
END 						; end of program
