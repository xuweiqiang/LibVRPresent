;*************************************************************************************
; Name:            IVI_YUV420ToRGB565ResizeRotate_8u16u_P3C3
; C Model API Interface
; iviRet IVI_YUV420ToRGB565ResizeRotate_8u16u_P3C3(
;	ivi8u *pSrc[3],ivi32s srcStride[3],ivi32s srcWidth,ivi32s srcHeight,
;	ivi16u *pDst,ivi32s dstStride,ivi32s dstWidth,ivi32s dstHeight,RotType rotationtype);
;   
;
;
; Description:     YUV420 to RGB565 Color Conversion 
; Input Arguments:	
;				pSrc                 -pointer of Source Y u v channel 
;				srcStride,           -stride of the source YUV channel 
;				pDst,				 -pointer of the RGB plane 
;				srcWidth,			 -the width of the original  iamge data  
;				srcHeight,			 -the height of the original  image data 
;				dstWidth,			 -the width of the destination  image data  
;				dstHeight,			 -the height of the destination  image data 
;				rotationtype		 -the rotation index .
;                                    0,no rotation ;
;                                    1,rotation 90;                    
;
; Output Arguments:
;                  
; Returns:         
;                   
; Notes:
;*************************************************************************************/
; R=Y+1.14*V           R=(Y*8+9V)>>3
; G=Y-0.394*U-0.581*V  G=(Y<<10-0.394*2^10*U-0.581*2^10*V)>>10)
; B=Y+2.032*U          B=(Y<<5+2.032*32*U)>>5


;r0-->rgb
;r1-->y
;r2-->u
;r3-->v
;r4-->9v/stride_rgb
;r5-->gu/adv_y
;r6-->gv/adv_u
;r7-->sv/adv_v
;r8-->r/srcWidth
;r9-->g/srcHeight
;r10-->SAT32_64_DK
;r11-->b
;r12 ->rotation/stride_y
;r14 ->


 AREA	CC_ROTATE, CODE, READONLY

 EXPORT   IVI_YUV420ToRGB565ResizeRotate_8u16u_P3C3
 
saturTable32_64
		DCB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ;-6
		DCB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ;-5
		DCB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ;-4
		DCB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ;-3
		DCB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ;;-2
		DCB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ;-1
		DCB	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 ;0
		DCB	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F ;1
		DCB	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 ;2
		DCB	0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F ;3
		DCB	0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F ;4
		DCB	0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F ;5
		DCB	0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F ;6
		DCB	0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F ;7
		DCB	0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F ;8
		DCB	0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F ;9
		DCB	0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F ;10
		DCB	0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F ;11

	
	
R_RANGE  EQU 5 ; Red
G_RANGE  EQU 6 ; Green
B_RANGE  EQU 5 ; Blue

Q10      EQU   10 ; assuse QBITS=10

GUGV     DCD  -403, -595  
 
in_width			EQU 0                ; mid _Width (must mutiply 8 )
in_height			EQU in_width+4       ; Height (must mutiply 8)
stride_y            EQU in_height+4      ; stride of y
stride_u            EQU stride_y+4      ; stride of u
stride_v            EQU stride_u+4      ; stride of u
real_stride_rgb     EQU stride_v + 4     ; real stride of rgb = stride_rgb * 2
real_adv_rgb        EQU real_stride_rgb+4; real rgb plane advancer   4 * stride_rgb - 2 * width 
adv_y               EQU real_adv_rgb+4   ; y plane advancer   2 * stride_y - width
adv_u               EQU adv_y+4          ; u plane advancer   stride_u - width/2
adv_v				EQU adv_u+4          ; v plane advancer   stride_v - width/2
top_start_90        EQU adv_v+4            ;the top row start point 
src_y    	    	EQU top_start_90+4   ; size of local data 
src_u    	    	EQU src_y+4   ; size of local data	
src_v               EQU src_u+4
temp_y              EQU src_v+4
stackSize           EQU temp_y+4
 

 MACRO
	DIV_MACRO $m, $r,$out
	
	RSBS       $m,$m,$r, LSR #15
	SUB        $m,$m,$r, LSR #15
	MOV        $m,$m,LSL #14
	ADDS       $r,$m,$r
	SUBCC      $r,$r,$m
	ADCS       $r,$m,$r,LSL #1
	SUBCC      $r,$r,$m
	ADCS       $r,$m,$r,LSL #1
	SUBCC      $r,$r,$m
	ADCS       $r,$m,$r,LSL #1
	SUBCC      $r,$r,$m
	ADCS       $r,$m,$r,LSL #1
	SUBCC      $r,$r,$m
	ADCS       $r,$m,$r,LSL #1
	SUBCC      $r,$r,$m
	ADCS       $r,$m,$r,LSL #1
	SUBCC      $r,$r,$m
	ADCS       $r,$m,$r,LSL #1
	SUBCC      $r,$r,$m
	ADCS       $r,$m,$r,LSL #1
	SUBCC      $r,$r,$m
	ADCS       $r,$m,$r,LSL #1
	SUBCC      $r,$r,$m
	ADCS       $r,$m,$r,LSL #1
	SUBCC      $r,$r,$m
	ADCS       $r,$m,$r,LSL #1
	SUBCC      $r,$r,$m
	ADCS       $r,$m,$r,LSL #1
	SUBCC      $r,$r,$m
	ADCS       $r,$m,$r,LSL #1
	SUBCC      $r,$r,$m
	ADCS       $r,$m,$r,LSL #1
	SUBCC      $r,$r,$m
	
	MOV        $r,$r,LSL #17
	MOV        $out , $r,LSR #16
	
MEND


IVI_YUV420ToRGB565ResizeRotate_8u16u_P3C3

    stmfd	sp!,{r4-r12,lr}             ; push r4-r12 & lr 
     ;;;get parameters form the fonction 
	mov		r8, r2                      ; in_Width
	mov		r9, r3                      ; in_Height
   	ldr     r2 , [sp , #40]             ; pDst
	ldr		r3 , [sp , #44]			    ; dstStride
	ldr     r6 , [sp , #48]             ; dstwidth 
	ldr     r7 , [sp , #52]             ; dstheight
	ldr		r10 , [sp , #56]			    ; Rotate
	
	mov     r11,r9,lsl #10
	mov     r12,r7
	DIV_MACRO  r12,r11,r5
	mov     r4,#0
    cmp			r10,#0
    beq         YUV420TORGB565_ROTATION_NO 
	cmp			r10,#1
	beq			YUV420TORGB565_ROTATION_90


YUV420TORGB565_ROTATION_NO
   mul		r10,r6,r9
   mul		r11,r7,r8
   
   cmp		r10,r11
   
   blt		V_NO
   bgt		H_NO
   mov     r8,r6
   mov     r9,r7
   b		RUN_NO
   
H_NO
  mov		r12,r9
  DIV_MACRO r12,r11,r8
  mov		r8,r8,LSR #2
  mov		r8,r8,LSL #2
  sub		r12,r6,r8
  add		r11,r2,r6,LSL #1
  mov		r10,r2
  mov		r9,r7
  
  sub		r11,r11,r12 
  
HLINE_NO

  mov		r14,r12
   
H_LOOP_NO

  subs		r14,r14,#4
  str		r4,[r10,r14]
  str		r4,[r11,r14]
  bne		H_LOOP_NO
  add		r10,r10,r3,LSL #1
  subs		r7,r7,#1
  add		r11,r11,r3,LSL #1
  bne		HLINE_NO
  
  add		r2,r2,r12  
  b			RUN_NO
  
V_NO
  mov		r12,r8,lsl #10
  mov		r14,r6
  DIV_MACRO r14,r12,r5
  
  mov		r12,r8
  DIV_MACRO r12,r10,r9
  mov		r9,r9,LSR #2
  mov		r9,r9,LSL #2
  
  
  sub		r12,r7,r9
  sub		r7,r7,#1
  mul		r14,r3,r7 
  add		r11,r2,r14,LSL #1
  mov		r10,r2
  mov		r8,r6
  mov		r12,r12,LSR #1   
  mov		r7,r12
  
VLINE_NO
  mov		r14,r6,LSL #2
V_LOOP_NO
  subs		r14,r14,#4
  str		r4,[r10,r14]
  str		r4,[r11,r14]
  bne		V_LOOP_NO
  add		r10,r10,r3,LSL #1
  subs		r7,r7,#1
  sub		r11,r11,r3,LSL #1
  bne		VLINE_NO
  mul		r12,r12,r3
  add		r2,r2,r12,LSL #1
 
  
RUN_NO
    sub		sp , sp , #stackSize	      ; allocate buffer for local valuables  
	str     r8,  [sp,#in_width]           ; push the width into buffer
    mov     r14 , r3 , lsl #1             ; real_stride_rgb = stride_rgb * 2
    ldr     r12 , [r1]					  ; r12 = stride_y
    str     r14 , [sp, #real_stride_rgb]  ; push real_stride_rgb
    str     r12 , [sp,#stride_y]          ; push stride_y
    sub     r4 , r14 , r8                 ; r4 = real_stride_rgb - width
	ldr     r6 , [r1 , #4]			      ; u_stride
	mov     r3 , r4 , lsl #1                ; r3 = real_adv_rgb 
	ldr     r7 , [r1 , #8]			      ; v_stride
	str     r3,[sp,#real_adv_rgb]		  ; save the real_adv_rgb
	str     r6,[sp,#stride_u]  
	str     r7,[sp,#stride_v]  
	   
	mov     r11,r2                  ; temp save dst
	ldr     r1,[r0]					; y
	ldr     r2,[r0,#4]				; u
	ldr     r3,[r0,#8]				; v
	mov     r0,r11                  ; RGB
    ldr         r10, =saturTable32_64
    add         r10, r10, #48
    str     r1,[sp,#src_y]
    str     r2,[sp,#src_u]
    str     r3,[sp,#src_v]
    mov     r14,#0
    str     r14,[sp,#temp_y]
    mov     r3,r5
    
LINE_0
    mov    r2,#0
    ldr    r7,[sp,#temp_y]
    mov    r6,r7,lsr #10
    
    ldr     r1,[sp,#src_y]
    ldr     r11,[sp,#src_u]
    ldr     r4,[sp,#stride_y] 
    ldr     r5,[sp,#stride_u]  
	
	mul    r12,r6,r4
	add    r12,r1,r12
	add    r7,r7,r3
	mov    r6,r7,lsr #10
	mul    r4,r6,r4
	add    r1,r1,r4
	add    r7,r7,r3
	str    r7,[sp,#temp_y]
	mov    r6,r6,lsr #1
	mul    r5,r6,r5
	add    r11,r11,r5
	str    r11,[sp,#adv_u]
	ldr    r7,[sp,#stride_v] 
	ldr    r11,[sp,#src_v] 
	mul    r7,r6,r7
	add    r11,r11,r7
	str     r1,[sp,#adv_y]
    str     r11,[sp,#adv_v]
	        
    		
ROW_0   
    ldr         r6,[sp,#adv_v]
    ldr         r7,[sp,#adv_u] 
     
    
	ldrb	     r4, [r6,r2,lsr # 11]		        ; v 
	ldrb	     r5, [r7,r2,lsr # 11]		        ; u
	ldr          r11,=GUGV
    	
	;;;;;;;u,v sub 128 
	sub		     r4,  r4,	#128			; u -= 128;
	sub          r5,  r5,	#128            ; v -= 128;
	
	;;;;;;r4*9, r5*0.392*1024(gu), r6*0.581*1024(gv), r7*2.032*32
	ldr     r1, [r11]                ; gu
	ldr     r14, [r11, #4]            ; gv 
	;add     r7, r5,r5,lsl #6          ; 65u
	mul     r7, r1,r5                ; gu*u 
	mul     r6, r14,r4                ; gv*v
	add     r4,r4,r4,lsl #3           ; 9v
	add     r7,r7,r6                 ; gu*u+gv*v
	
	;;;;;r6 is free, may use it to load y,convert
	ldrb	r6, [r12,r2,lsr #10]				 ; y = pSrcY[0]
	add     r11, r4,r6,lsl #3		 ; r = (y<<3) + 9v
	add     r1, r7,r6,lsl #Q10       ; g = (y<<Q10) + K[1]u + K[2]v
	add     r6,r6,r5,lsl #1         ; b = (y<<5) + 65u

	;;;;clip the range of it          
	ldrb     r11,	[r10,  r11, asr #3+8-R_RANGE]		; R = SAT32_DK[r>>(3+3)]
	ldrb     r6,	[r10,  r6,  asr #8-B_RANGE]		; B = SAT32_DK[b>>(5+3)]
	ldrb     r1,	[r10, r1,asr #Q10+9-G_RANGE]		                    ; G = SAT64_DK[g>>(Q10+2)]
	
	
    ;;;;r3 is the value of rgb565
    orr      r11, r1, r11,	lsl #5
	orr      r14,  r6, r11,lsl #6
    
    add  r2,r2,r3
	;;;count the r0+1
	ldrb	 r6,[r12,r2,lsr #10]		                            ; y = pSrcY[1]
	add      r11, r4,r6,lsl #3		                    ; r = (y<<3) + 9v
	add      r1, r7,r6,lsl #Q10                        ; g = (y<<Q10) + K[1]u + K[2]v
	add      r6,  r6,r5,lsl #1                          ; b = (y<<5) + 65u
    
	ldrb     r11,	[r10,  r11, asr #3+8-R_RANGE]		; R = SAT32_DK[r>>(3+3)]
	ldrb     r1,	[r10,  r1,asr #Q10+9-G_RANGE]		                    ; G = SAT64_DK[g>>(Q10+2)]
	ldrb     r6,	[r10,  r6,  asr #8-B_RANGE]		; B = SAT32_DK[b>>(5+3)]

    orr      r1, r1, r11,lsl #5
    orr      r6, r6, r1,lsl #6
    orr      r14, r14,r6,lsl #16
    str      r14,[r0]                                  ;
    
    ;;;;;count the second row
    
    sub      r2,r2,r3
    
    ldr      r14, [sp, #adv_y]                        ;r14 = stride_y
    ldrb     r6,[r14,r2,lsr #10]                                 ;y[]
	add      r11, r4,r6,asl #3		                     ; r = (y<<3) + 9v
	add      r1, r7,r6,asl #Q10                         ; g = (y<<Q10) + K[1]u + K[2]v
	add      r6, r6,r5,asl #1                            ; b = (y<<5) + 65u
	ldrb     r11,	[r10, r11, asr #3+8-R_RANGE]		 ; R = SAT32_DK[r>>(3+3)]
    ldrb     r1,  [r10, r1,asr #Q10+9-G_RANGE]	                         ; G = SAT64_DK[g>>(Q10+2)]
	ldrb     r6, [r10, r6, asr #8-B_RANGE]		     ; B = SAT32_DK[b>>(5+3)]
    orr      r1,r1,r11,  lsl #5
    mov      r11,r14
    orr      r14,r6,r1,	lsl #6
    
    add      r2,r2,r3
    ;;;;;;count the second row the second col
    ldrb     r6, [r11,r2,lsr #10]                               ; r6 is y
    add      r11, r4,r6,asl #3		                     ; r = (y<<3) + 9v
	add      r1, r7,r6,asl #Q10                         ; g = (y<<Q10) + K[1]u + K[2]v
	add      r6,r6,r5,asl #1                             ; b = (y<<5) + 65u   
	ldrb     r11,	[r10,  r11, asr #3+8-R_RANGE]		 ; R = SAT32_DK[r>>(3+3)]
	ldrb     r1,	[r10, r1,asr #Q10+9-G_RANGE]		                    ; G = SAT64_DK[g>>(Q10+2)]
	ldrb     r6,	[r10,  r6,asr #8-B_RANGE]		    ; B = SAT32_DK[b>>(5+3)]
	orr      r1,    r1,r11,lsl #5
    orr      r1,    r6,r1,	lsl #6
    ldr      r6,  [sp, #real_stride_rgb] 
    orr      r14, r14,r1,lsl #16

    subs     r8 , r8,#2
    str      r14,[r0, r6]
    add      r0 , r0 , #4  
    add      r2,r2,r3                                
 
    bne      ROW_0
    ldr      r4, [sp, #real_adv_rgb]
    add      r0 , r0, r4       
    ldr      r8 , [sp, #in_width]
    subs     r9,  r9 , #2

	bne      LINE_0
	B        OVER  
	
YUV420TORGB565_ROTATION_90

   mul		r10,r6,r9
   mul		r11,r7,r8
   cmp		r10,r11
   blt		V_90
   bgt		H_90
   mov     r8,r6
   mov     r9,r7
   b		RUN_90
   
H_90
   mov		r12,r9
   DIV_MACRO r12,r11,r8
   mov		r8,r8,LSR #2
   mov		r8,r8,LSL #2
   sub		r12,r6,r8
   sub		r6,r6,#1
   mul		r14,r3,r6  
   add		r11,r2,r14,LSL #1
   mov      r10,r2
   mov		r9,r7
   mov		r7,r12, LSR #1
 
  
HLINE_90
   mov		r14,r9,LSL #2
H_LOOP_90
   subs		r14,r14,#4
   str		r4,[r10,r14]
   str		r4,[r11,r14]
   bne		H_LOOP_90
   add		r10,r10,r3,LSL #1
   subs		r7,r7,#1
   sub		r11,r11,r3,LSL #1
   bne		HLINE_90
   mul		r12,r12,r3
   add		r2,r2,r12
   b		RUN_90
  
V_90
   mov		r12,r8,lsl #10
   mov		r14,r6
   DIV_MACRO  r14,r12,r5
   mov		r12,r8
   DIV_MACRO r12,r10,r9
   mov		r9,r9,LSR #2
   mov		r9,r9,LSL #2
   sub		r12,r7,r9
   add		r11,r2,r7,LSL #1
   mov		r10,r2
   mov		r8,r6
   sub		r11,r11,r12 
  
VLINE_90
   mov		r14,r12 
V_LOOP_90
   subs		r14,r14,#4
   str		r4,[r10,r14]
   str		r4,[r11,r14]
   bne		V_LOOP_90
   add		r10,r10,r3,LSL #1
   subs		r6,r6,#1
   add		r11,r11,r3,LSL #1
   bne		VLINE_90
   add		r2,r2,r12  
  

  
RUN_90

   sub  r2,r2,#4
   add  r2,r2,r9,LSL #1
  
    sub		sp , sp , #stackSize	      ; allocate buffer for local valuables   
	str     r8,  [sp,#in_width]           ; push the width into buffer
    mov     r14 , r3 , lsl #1             ; real_stride_rgb = stride_rgb * 2
    ldr     r12 , [r1]					  ; r12 = stride_y
    str     r14 , [sp, #real_stride_rgb]  ; push real_stride_rgb
    str     r12 , [sp,#stride_y]          ; push stride_y
    sub     r4 , r14 , r8                 ; r4 = real_stride_rgb - width
	ldr     r6 , [r1 , #4]			      ; u_stride
	mov     r3 , r4 , lsl #1                ; r3 = real_adv_rgb 
	ldr     r7 , [r1 , #8]			      ; v_stride
	str     r3,[sp,#real_adv_rgb]		  ; save the real_adv_rgb
	str     r6,[sp,#stride_u]  
	str     r7,[sp,#stride_v]  

	mov     r11,r2                  ; temp save dst
	ldr     r1,[r0]					; y
	ldr     r2,[r0,#4]				; u
	ldr     r3,[r0,#8]				; v
	mov     r0,r11                  ; RGB
    ldr         r10, =saturTable32_64
    add         r10, r10, #48
    str     r1,[sp,#src_y]
    str     r2,[sp,#src_u]
    str     r3,[sp,#src_v]
    mov     r14,#0
    str     r14,[sp,#temp_y]
    mov     r3,r5
    str     r0, [sp, #top_start_90]
    
LINE_90
    mov    r2,#0
    ldr    r7,[sp,#temp_y]
    mov    r6,r7,lsr #10
    
    ldr    r1,[sp,#src_y]
    ldr    r11,[sp,#src_u]
   
    
    ldr    r4,[sp,#stride_y] 
    ldr    r5,[sp,#stride_u]  
	mul    r12,r6,r4
	add    r12,r1,r12
	add    r7,r7,r3
	mov    r6,r7,lsr #10
	mul    r4,r6,r4
	add    r1,r1,r4
	add    r7,r7,r3
	str    r7,[sp,#temp_y]
	mov    r6,r6,lsr #1
	mul    r5,r6,r5
	add    r11,r11,r5
	str    r11,[sp,#adv_u]
	ldr    r7,[sp,#stride_v] 
	ldr    r11,[sp,#src_v] 
	mul    r7,r6,r7
	add    r11,r11,r7
	str     r1,[sp,#adv_y]
    str     r11,[sp,#adv_v]

    
        
ROW_90
      
	ldr         r6,[sp,#adv_v]
    ldr         r7,[sp,#adv_u] 
     
    
	ldrb	     r4, [r6,r2,lsr # 11]		        ; v 
	ldrb	     r5, [r7,r2,lsr # 11]		        ; u
	ldr          r11,=GUGV
    	
	;;;;;;;u,v sub 128 
	sub		     r4,  r4,	#128			; u -= 128;
	sub          r5,  r5,	#128            ; v -= 128;
	
	;;;;;;r4*9, r5*0.392*1024(gu), r6*0.581*1024(gv), r7*2.032*32
	ldr     r1, [r11]                ; gu
	ldr     r14, [r11, #4]            ; gv 
	mul     r7, r1,r5                ; gu*u 
	mul     r6, r14,r4                ; gv*v
	add     r4,r4,r4,lsl #3           ; 9v
	add     r7,r7,r6                 ; gu*u+gv*v
	
	;;;;;r6 is free, may use it to load y,convert
	ldrb	r6, [r12,r2,lsr #10]				 ; y = pSrcY[0]
	add     r11, r4,r6,lsl #3		 ; r = (y<<3) + 9v
	add     r1, r7,r6,lsl #Q10       ; g = (y<<Q10) + K[1]u + K[2]v
	add     r6,r6,r5,lsl #1         ; b = (y<<5) + 65u

	;;;;clip the range of it          
	ldrb     r11,	[r10,  r11, asr #3+8-R_RANGE]		; R = SAT32_DK[r>>(3+3)]
	ldrb     r6,	[r10,  r6,  asr #8-B_RANGE]		; B = SAT32_DK[b>>(5+3)]
	ldrb     r1,	[r10, r1,asr #Q10+9-G_RANGE]		                    ; G = SAT64_DK[g>>(Q10+2)]
	
    orr      r11, r1, r11,	lsl #5
	orr      r14,  r6, r11,lsl #6
	
	
	ldr      r11, [sp, #adv_y]                        ;r14 = stride_y
    ldrb     r6,[r11,r2,lsr #10]                                 ;y[]
	add      r11, r4,r6,asl #3		                     ; r = (y<<3) + 9v
	add      r1, r7,r6,asl #Q10                         ; g = (y<<Q10) + K[1]u + K[2]v
	add      r6, r6,r5,asl #1                            ; b = (y<<5) + 65u
	ldrb     r11,	[r10, r11, asr #3+8-R_RANGE]		 ; R = SAT32_DK[r>>(3+3)]
    ldrb     r1,  [r10, r1,asr #Q10+9-G_RANGE]	                         ; G = SAT64_DK[g>>(Q10+2)]
	ldrb     r6, [r10, r6, asr #8-B_RANGE]		     ; B = SAT32_DK[b>>(5+3)]
    orr      r1,r1,r11,  lsl #5
    orr      r11,r6,r1,	lsl #6
    orr      r14,r11 ,r14,lsl #16
    
    str      r14,[r0]
    
    ldr      r11, [sp, #adv_y]  
    add      r2,r2,r3
   
    ldrb     r6, [r11,r2,lsr #10]                               ; r6 is y
    add      r11, r4,r6,asl #3		                     ; r = (y<<3) + 9v
	add      r1, r7,r6,asl #Q10                         ; g = (y<<Q10) + K[1]u + K[2]v
	add      r6,r6,r5,asl #1                             ; b = (y<<5) + 65u   
	ldrb     r11,	[r10,  r11, asr #3+8-R_RANGE]		 ; R = SAT32_DK[r>>(3+3)]
	ldrb     r1,	[r10, r1,asr #Q10+9-G_RANGE]		                    ; G = SAT64_DK[g>>(Q10+2)]
	ldrb     r6,	[r10,  r6,asr #8-B_RANGE]		    ; B = SAT32_DK[b>>(5+3)]
	orr      r1,    r1,r11,lsl #5
    orr      r14,   r6,r1,	lsl #6

	ldrb	 r6,[r12,r2,lsr #10]		                            ; y = pSrcY[1]
	add      r11, r4,r6,lsl #3		                    ; r = (y<<3) + 9v
	add      r1, r7,r6,lsl #Q10                        ; g = (y<<Q10) + K[1]u + K[2]v
	add      r6,  r6,r5,lsl #1                          ; b = (y<<5) + 65u
    
	ldrb     r11,	[r10,  r11, asr #3+8-R_RANGE]		; R = SAT32_DK[r>>(3+3)]
	ldrb     r1,	[r10,  r1,asr #Q10+9-G_RANGE]		                    ; G = SAT64_DK[g>>(Q10+2)]
	ldrb     r6,	[r10,  r6,  asr #8-B_RANGE]		; B = SAT32_DK[b>>(5+3)]

    orr      r1, r1, r11,lsl #5
    orr      r6, r6, r1,lsl #6
    ldr      r5,  [sp, #real_stride_rgb] 
    orr      r14,r14,r6,lsl #16
   
    str      r14,[r0,r5]                                  ;
    
    ;;;;;count the second row
    add      r2,r2,r3 
    add      r0 , r0 , r5,lsl #1        
    subs     r8 , r8,#2
     
    bne      ROW_90
    
    ldr      r4, [sp, #top_start_90]
    subs     r9,  r9 , #2
    sub      r0 , r4, #4 
    str      r0 ,[sp , #top_start_90]
    ldr      r8 ,[sp, #in_width] 
 
  	bne      LINE_90
	
	B        OVER
   
	
OVER 

	add		sp,		sp,		#stackSize
	mov		r0,		#0
	ldmfd	sp!,	{r4-r12,pc}
    



    END
	






















