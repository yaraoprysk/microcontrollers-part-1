;v16

;port-A, port-F
;T5


.nolist
.include "m2560def.inc"
.list



;===FLASH===
    .CSEG;vector interrupts
    .org 0x00
  rjmp reset  
    .org OC5Aaddr
  rjmp TIMER5_COMPA;Timer1 interrupt

TIMER5_COMPA:
  in r16, SREG
  push r16
  sbrs r0, 0
  rjmp algorithm_2
  cpi r22, 4; algorithm 1
  brlo continue
  nop
  cpi r22, 5; algorithm 1_2
  brge continue_2
  continue_2:
    lsr r27
    out PORTF, r27 
    lsr r27
    inc r22
    rjmp algorithm_2
  
    
  
  clt
  bld r0, 0
  rjmp algorithm_2
  
  continue:
    out PORTF, r20 
  lsr r20
    lsr r20
  inc r22

algorithm_2:
  sbrs r0, 1
  rjmp return
  cpi r23, 4
  brlo continue_3  
  clr r21
  out PORTA, r21
  clt
  bld r0, 1
  rjmp return
  continue_3:
    out PORTA, r21
    lsl r18
    lsr r19
    mov r21, r18
    or  r21, r19  
    inc r23
return:
  pop r16
  out SREG, r16
  reti
;--------------------------------------------------------------------
reset:
  ldi  r16, low(RAMEND)
  out SPL, r16
  ldi r16, high(RAMEND)
  out SPH, r16
  ;setting io ports
  ldi r16, 0xFF
  ldi r17, 0x00
  out DDRA, r16;0xFF вихід

  ;
  out DDRF, r16;0xFF вихід
  out PORTA, r17;0x00 низ.деф
  out PORTF, r17;0x00 низ.деф
  ;

  ldi r16, (1<<DDC5) | (1<<DDC7)
  out PORTC, r16
  
 
  ;-----------buzzer
  sbi DDRC, 0
  cbi PORTC, 0
  ;-----------
  ldi r16, 0x0C
  ldi r17, 0x34
  sts  OCR5AH, r16
  sts OCR5AL, r17
  ldi r16, 0x00
  sts TCCR5A, r16
   ldi r16, (1 << WGM52) | (1 << CS52) | (1 << CS50)
  sts TCCR5B, r16
  ldi r16, (1 << OCIE5A)
  sts TIMSK5, r16

  ;clear registers
  clr r0
  clr r16
  clr r17
  clr r18
  clr r19
  clr r20
  clr r27
  clr r22
  clr r21
  clr r23
  clr r24      
  clr r25         
  clr r26
  sei 
;----------------------------------------------------------------
main:  
    clt
    sbis PINC, 5
    rjmp next_button
    rcall delay
 
    sbis PINC, 5
    rcall button_1_is_pressed
  next_button:
    sbis PINC, 7
    rjmp main
    rcall delay
     sbis PINC, 7
    rcall button_2_is_pressed
    rjmp main

button_1_is_pressed:
    clr r22
    ldi r20, 1<< 7
    ldi r27,1 << 7
    set
    bld r0, 0
    rcall buzzer
    ret

button_2_is_pressed:
    clr r23
    ldi r18, 1
    ldi r19, 1 << 7
    mov r21, r18
    or  r21, r19
    set
    bld r0, 1
    rcall buzzer
    ret
  
buzzer:
  sbi  PORTC, 0
  rcall delay
  cbi  PORTC, 0

  ret  
       
delay:       
  ldi r24, 0xFF      
  ldi r25, 0xFF
  ldi r26, 1 
  del:                
    subi r24, 1       
    sbci r25, 0          
    sbci r26, 0  
  brcc del     
ret               

; EEPROM
  .ESEG