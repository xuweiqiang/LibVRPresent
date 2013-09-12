;*************************************************************************************
; Name:            IVI_YUV420ToRGB565Rotate_8u16u_P3C3
; C Model API Interface
; iviRet IVI_YUV420ToRGB565Rotate_8u16u_P3C3(
;	ivi8u *pSrc[3],ivi32s srcStride[3],ivi32s srcWidth,ivi32s srcHeight,
;	ivi16u *pDst,ivi32s dstStride,RotType rotationtype);
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
;				rotationtype		 -the rotation index .
;                                    0,no rotation ;
;                                    1,rotation 90;
;                                    2,rotation 180;
;                                    3,rotation 270;;                    
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
 EXPORT   IVI_YUV420ToRGB565Rotate_8u16u_P3C3
 
saturTable32_64_ARM
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

		DCB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ;-12
		DCB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ;-11
		DCB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ;-10
		DCB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ;-9
		DCB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ;-8
		DCB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ;-7
		DCB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ;-6
		DCB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ;-5
		DCB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ;-4
		DCB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ;-3
		DCB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ;-2
		DCB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ;-1
		DCB	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 ;0
		DCB	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F ;1
		DCB	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 ;2
		DCB	0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F ;3
		DCB	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27 ;4
		DCB	0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F ;5
		DCB	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37 ;6
		DCB	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F ;7
		DCB	0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F ;8
		DCB	0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F ;9
		DCB	0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F ;10
		DCB	0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F ;11
		DCB	0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F ;12
		DCB	0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F ;13
		DCB	0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F ;14
		DCB	0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F ;15
		DCB	0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F ;16
		DCB	0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F ;17
		DCB	0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F ;18
		DCB	0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F ;19
		DCB	0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F ;20
		DCB	0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F ;21
		DCB	0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F ;22
		DCB	0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F ;23
	
R_RANGE  EQU 5 ; Red
G_RANGE  EQU 6 ; Green
B_RANGE  EQU 5 ; Blue

Q10      EQU   10 ; assuse QBITS=10

GUGV     DCD  -403, -595  
 
in_width			EQU 0                ; mid _Width (must mutiply 8 )
in_height			EQU in_width+4       ; Height (must mutiply 8)
stride_y            EQU in_height+4      ; stride of y
real_stride_rgb     EQU stride_y + 4     ; real stride of rgb = stride_rgb * 2
real_adv_rgb        EQU real_stride_rgb+4; real rgb plane advancer   4 * stride_rgb - 2 * width 
adv_y               EQU real_adv_rgb+4   ; y plane advancer   2 * stride_y - width
adv_u               EQU adv_y+4          ; u plane advancer   stride_u - width/2
adv_v				EQU adv_u+4          ; v plane advancer   stride_v - width/2
top_start_90        EQU adv_v+4            ;the top row start point  
stackSize	    	EQU top_start_90+4   ; size of local data	

     ALIGN 4

IVI_YUV420ToRGB565Rotate_8u16u_P3C3
    
    stmfd	sp!,{r4-r12,lr}             ; push r4-r12 & lr 
     ;;;get parameters form the fonction 
	mov		r8, r2                      ; in_Width
	mov		r9, r3                      ; in_Height
   	ldr     r2 , [sp , #40]             ; pDst
	ldr		r3 , [sp , #44]			    ; dstStride
    ldr     r10 ,[sp , #48] 
	   
    cmp			r10,#0
    beq         YUV420TORGB565_ROTATION_NO 
	cmp			r10,#1
	beq			YUV420TORGB565_ROTATION_90
   
YUV420TORGB565_ROTATION_NO
    sub		sp , sp , #stackSize	      ; allocate buffer for local valuables
	str     r8,  [sp,#in_width]           ; push the width into buffer
	str     r9,  [sp,#in_height]          ; push the height into buffer

    mov     r14 , r3 , lsl #1             ; real_stride_rgb = stride_rgb * 2
    ldr     r12 , [r1]              ; r12 = stride_y
    str     r14 , [sp, #real_stride_rgb]  ; push real_stride_rgb
    str     r12 , [sp,#stride_y]          ; push stride_y
    sub     r4 , r14 , r8               ; r4 = real_stride_rgb - width
	ldr     r6 , [r1 , #4]			; u_stride
	mov     r3 , r4 , lsl #1                ; r3 = real_adv_rgb 
	ldr     r7 , [r1 , #8]			; v_stride
	str     r3,[sp,#real_adv_rgb]       ; save the real_adv_rgb
	
	rsb     r5 , r8, r12, lsl #1       ; adv_y = stride_y * 2 - width
	sub     r6 , r6,r8,lsr #1         ; adv_u = stride_u-width/2
	str     r5 , [sp, #adv_y]         ; save the adv_y data
	sub     r7 , r7,r8,lsr #1         ; adv_v	= stride_v-width/2
    str     r6 , [sp, #adv_u]         ; save the adv_u data
    str     r7 , [sp, #adv_v]         ; save the adv_v data
    
	mov     r11,r2                  ; temp save dst
	ldr     r1,[r0]					; y
	ldr     r2,[r0,#4]				; u
	ldr     r3,[r0,#8]				; v
	mov     r0,r11                  ; RGB
    ldr         r10, =saturTable32_64_ARM
    add         r10, r10, #48
    		
ROW_0
      
	ldrb	     r4, [r3],#1		        ; v 
	ldrb	     r5, [r2],#1		        ; u
	ldr          r11,=GUGV
    	
	;;;;;;;u,v sub 128 
	sub		     r4,  r4,	#128			; u -= 128;
	sub          r5,  r5,	#128            ; v -= 128;
	
	;;;;;;r4*9, r5*0.392*1024(gu), r6*0.581*1024(gv), r7*2.032*32
	ldr     r12, [r11]                ; gu
	ldr     r14, [r11, #4]            ; gv 
	add     r7, r5,r5,asl #6          ; 65u
	mul     r5, r12,r5                ; gu*u 
	mul     r6, r14,r4                ; gv*v
	add     r4,r4,r4,asl #3           ; 9v
	add     r5,r5,r6                 ; gu*u+gv*v
	
	;;;;;r6 is free, may use it to load y,convert
	ldrb	r6, [r1]				 ; y = pSrcY[0]
	add     r11, r4,r6,lsl #3		 ; r = (y<<3) + 9v
	add     r12, r5,r6,lsl #Q10       ; g = (y<<Q10) + K[1]u + K[2]v
	add     r6,r7,r6,lsl #5         ; b = (y<<5) + 65u

	;;;;clip the range of it 
	mov      r12,   r12,  asr #Q10+8-G_RANGE          
	ldrb     r11,	[r10,  r11, asr #3+8-R_RANGE]		; R = SAT32_DK[r>>(3+3)]
	add      r12,   r12,  #192
	ldrb     r6,	[r10,  r6,  asr #5+8-B_RANGE]		; B = SAT32_DK[b>>(5+3)]

	ldrb     r12,	[r10, r12]		                    ; G = SAT64_DK[g>>(Q10+2)]
	
	
    ;;;;r3 is the value of rgb565
    orr      r11, r12, r11,	lsl #G_RANGE
	orr      r14,  r6, r11,lsl #B_RANGE
    
	;;;count the r0+1
	ldrb	 r6,[r1,#1]		                            ; y = pSrcY[1]
	add      r11, r4,r6,lsl #3		                    ; r = (y<<3) + 9v
	add      r12, r5,r6,lsl #Q10                        ; g = (y<<Q10) + K[1]u + K[2]v
	add      r6,  r7,r6,lsl #5                          ; b = (y<<5) + 65u
    
	mov      r12,   r12,  asr #Q10+8-G_RANGE 
	ldrb     r11,	[r10,  r11, asr #3+8-R_RANGE]		; R = SAT32_DK[r>>(3+3)]
	add      r12,   r12,  #192
	ldrb     r12,	[r10, r12]		                    ; G = SAT64_DK[g>>(Q10+2)]
	ldrb     r6,	[r10,  r6,  asr #5+8-B_RANGE]		; B = SAT32_DK[b>>(5+3)]

    orr      r12, r12, r11,lsl #G_RANGE
    orr      r6, r6, r12,lsl #B_RANGE
    orr      r14, r14,r6,lsl #16
    str      r14,[r0]                                  ;
    
    ;;;;;count the second row
    ldr      r14, [sp, #stride_y]                        ;r14 = stride_y
    ldrb     r6,[r1,r14]                                 ;y[]
	add      r11, r4,r6,asl #3		                     ; r = (y<<3) + 9v
	add      r12, r5,r6,asl #Q10                         ; g = (y<<Q10) + K[1]u + K[2]v
	add      r6, r7,r6,asl #5                            ; b = (y<<5) + 65u
	mov      r12,  r12, asr #Q10+8-G_RANGE 
	ldrb     r11,	[r10, r11, asr #3+8-R_RANGE]		 ; R = SAT32_DK[r>>(3+3)]
	add      r12,  r12, #192
    ldrb     r12,  [r10, r12]	                         ; G = SAT64_DK[g>>(Q10+2)]
	ldrb     r6, [r10, r6, asr #5+8-B_RANGE]		     ; B = SAT32_DK[b>>(5+3)]
    orr      r12,r12,r11,  lsl #G_RANGE
    add      r11, r14, #1
    orr      r14,r6,r12,	lsl #B_RANGE
    
    ;;;;;;count the second row the second col
    ldrb     r6, [r1, r11]                               ; r6 is y
    add      r11, r4,r6,asl #3		                     ; r = (y<<3) + 9v
	add      r12, r5,r6,asl #Q10                         ; g = (y<<Q10) + K[1]u + K[2]v
	add      r6,r7,r6,asl #5                             ; b = (y<<5) + 65u
	mov      r12, r12,asr #Q10+8-G_RANGE    
	ldrb     r11,	[r10,  r11, asr #3+8-R_RANGE]		 ; R = SAT32_DK[r>>(3+3)]
    add      r12, r12,#192
	ldrb     r12,	[r10, r12]		                    ; G = SAT64_DK[g>>(Q10+2)]
	ldrb     r6,	[r10,  r6,asr #5+8-B_RANGE]		    ; B = SAT32_DK[b>>(5+3)]
	orr      r12,    r12,r11,lsl #G_RANGE
    orr      r12,    r6,r12,	lsl #B_RANGE
    ldr      r6,  [sp, #real_stride_rgb] 
    orr      r14, r14,r12,lsl #16

    add      r1 , r1 , #2
    subs     r8 , r8,#2
    str      r14,[r0, r6]
    add      r0 , r0 , #4  
                                    
 
    bne      ROW_0
    ldr      r4, [sp, #real_adv_rgb]
    ldr      r5, [sp, #adv_y]
    ldr      r6, [sp, #adv_u]
    ldr      r7, [sp, #adv_v]
    add      r0 , r0, r4 
    add      r1 , r1 ,r5
    add      r2 , r2 ,r6
    add      r3 , r3 ,r7   
    ldr      r8 , [sp, #in_width]
    subs     r9,  r9 , #2

	bne      ROW_0
	B        OVER



YUV420TORGB565_ROTATION_90
    sub		sp , sp , #stackSize	    ; allocate buffer for local valuables
	mov     r14 , r3 , lsl #1           ; real_stride_rgb = height * 2
	str     r8,  [sp,#in_width]           ; push the width into buffer
	str     r9,  [sp,#in_height]          ; push the height into buffer
	str     r14,[sp,#real_stride_rgb]       ; save the real_adv_rgb
	
	ldr     r12 , [r1]                  ; r12 = stride_y
	ldr     r6 , [r1 , #4]			    ; u_stride
	ldr     r7 , [r1 , #8]			    ; v_stride
	str     r12 , [sp, #stride_y]
	
	rsb     r5 , r8, r12, lsl #1       ; adv_y = stride_y * 2 - width
	sub     r6 , r6,r8,lsr #1         ; adv_u = stride_u-width/2
	sub     r7 , r7,r8,lsr #1         ; adv_v	= stride_v-width/2
	str     r5 , [sp, #adv_y]         ; save the adv_y data
	str     r6 , [sp, #adv_u]         ; save the adv_u data
    str     r7 , [sp, #adv_v]         ; save the adv_v data
    
    mov     r11,  r9,lsl #1
	sub     r11 , r11 , #4             ; get the top start point 
	
	ldr     r1, [r0]
	ldr     r3, [r0,#8]				; v
	add     r11 , r11 , r2 
	ldr     r10, =saturTable32_64_ARM           ;
	str     r11 , [sp, #top_start_90]  ; save the offset of start point
	add     r10, r10, #48 
	ldr     r2, [r0,#4]				; u
	mov     r0 , r11                 ; move the RGB position 
        
ROW_90
      
	ldrb	     r4, [r3],#1		        ; v 
	ldrb	     r5, [r2],#1		        ; u
	ldr          r11,=GUGV
    	
	;;;;;;;u,v sub 128 
	sub		     r4,  r4,	#128			; v -= 128;
	sub          r5,  r5,	#128            ; u -= 128;
	
	;;;;;;r4*9, r5*0.392*1024(gu), r6*0.581*1024(gv), r7*2.032*32
	ldr     r12, [r11]                  ; gu
	ldr     r14, [r11, #4]              ; gv 
	add     r7,  r5,r5,asl #6           ; 65u
	mul     r5,  r12,r5                 ; gu*u 
	mul     r6,  r14,r4                 ; gv*v
	add     r4,  r4,r4,asl #3           ; 9v
	add     r5,  r5,r6                  ; gu*u+gv*v
	
	;;;;;r6 is free, may use it to load y,convert
	ldrb	r6, [r1]				 ; y = pSrcY[0]
	add     r11, r4,r6,lsl #3		 ; r = (y<<3) + 9v
	add     r12, r5,r6,lsl #Q10       ; g = (y<<Q10) + K[1]u + K[2]v
	add     r6,r7,r6,lsl #5         ; b = (y<<5) + 65u

	;;;;clip the range of it 
	mov      r12,   r12,  asr #Q10+8-G_RANGE          ;
	ldrb     r11,	[r10,  r11, asr #3+8-R_RANGE]		; R = SAT32_DK[r>>(3+3)]
	add      r12,   r12,  #192
	ldrb     r6,	[r10,  r6,  asr #5+8-B_RANGE]		; B = SAT32_DK[b>>(5+3)]
	ldrb     r12,	[r10, r12]		                    ; G = SAT64_DK[g>>(Q10+2)]
	
    ;;;;r3 is the value of rgb565
    orr      r12,r12,r11,	lsl #G_RANGE
    orr      r14,r6,r12,	lsl #B_RANGE
    
	;;;;;count the second row
	ldr      r12 , [sp, #stride_y]
    ldrb     r6,   [r1,r12]                                 ;y[]
	add      r11,  r4,r6,asl #3		                     ; r = (y<<3) + 9v
	add      r12,  r5,r6,asl #Q10                          ; g = (y<<Q10) + K[1]u + K[2]v
	add      r6,   r7,r6,asl #5                            ; b = (y<<5) + 65u
	mov      r12,  r12, asr #Q10+8-G_RANGE              
	ldrb     r11,  [r10, r11, asr #3+8-R_RANGE]		 ; R = SAT32_DK[r>>(3+3)]
	add      r12,  r12, #192
    ldrb     r6,  [r10, r6, asr #5+8-B_RANGE]		     ; B = SAT32_DK[b>>(5+3)]
    ldrb     r12,  [r10, r12]	                         ; G = SAT64_DK[g>>(Q10+2)]
    
    orr      r12, r12,r11,  lsl #G_RANGE
    orr      r6,  r6,r12,	lsl #B_RANGE
    orr      r14, r6,r14,lsl #16
    ldr      r12 , [sp, #real_stride_rgb]
    str      r14, [r0]                                  ;
    ldrb	 r6,[r1,#1]		                            ; y = pSrcY[1]

    add      r0, r0, r12                                ;ro is the new row
	;;;count the r0+1

	add      r11, r4,r6,lsl #3		                    ; r = (y<<3) + 9v
	add      r12, r5,r6,lsl #Q10                          ; g = (y<<Q10) + K[1]u + K[2]v
	add      r6,r7,r6,lsl #5                            ; b = (y<<5) + 65u
    
	mov      r12,   r12,  asr #Q10+8-G_RANGE          ;
	ldrb     r11,	[r10,  r11, asr #3+8-R_RANGE]		; R = SAT32_DK[r>>(3+3)]
	add      r12,   r12,  #192
	ldrb     r6,	[r10,  r6,  asr #5+8-B_RANGE]		; B = SAT32_DK[b>>(5+3)]
	ldrb     r12,	[r10, r12]	 ; G = SAT64_DK[g>>(Q10+2)]
	ldr      r14, [sp,#stride_y]
    orr      r11, r12,  r11,lsl #G_RANGE
    orr      r12, r6, r11,lsl #B_RANGE
    
    ;;;;;;count the second row the second col
  
    add      r11 , r14, #1
    ldrb     r6, [r1, r11]  
    mov      r14, r12
	add      r11, r4,r6,asl #3		                     ; r = (y<<3) + 9v
	add      r12, r5,r6,asl #Q10                          ; g = (y<<Q10) + K[1]u + K[2]v
	add      r6,r7,r6,asl #5                            ; b = (y<<5) + 65u
	mov      r12, r12,asr #Q10+8-G_RANGE 
	ldrb     r11,	[r10,  r11, asr #3+8-R_RANGE]		; R = SAT32_DK[r>>(3+3)]
	add      r12, r12,#192
	ldrb     r6,	[r10,  r6,asr #5+8-B_RANGE]		    ; B = SAT32_DK[b>>(5+3)]
	ldrb     r12,	[r10, r12]	          ; G = SAT64_DK[g>>(Q10+2)]
	orr      r11,    r12,r11,lsl #G_RANGE
    orr      r6,    r6,r11,	lsl #B_RANGE
    ldr      r12, [sp, #real_stride_rgb]
    orr      r14, r6,r14,lsl #16
                                     
    add      r1 , r1 , #2
    str      r14,[r0]
    add      r0 , r0 , r12        
    subs     r8 , r8,#2
      
    bne      ROW_90
    
    ldr      r4, [sp, #top_start_90]
    ldr      r6, [sp, #adv_u]
    sub      r0 , r4, #4 
    ldr      r5, [sp, #adv_y]
    ldr      r7, [sp, #adv_v]
    str      r0 , [sp , #top_start_90]
    add      r1 , r1 ,r5
    add      r2 , r2 ,r6
    add      r3 , r3 ,r7
    ldr      r8 ,[sp, #in_width] 
    subs     r9,  r9 , #2
  
  	bne      ROW_90
	
	B        OVER


OVER 

	add		sp,		sp,		#stackSize
	mov		r0,		#0
	ldmfd	sp!,	{r4-r12,pc}

    END
	






















