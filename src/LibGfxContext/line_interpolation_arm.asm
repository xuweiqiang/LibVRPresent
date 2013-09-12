;*************************************************************************************
;void line_intepolation(unsigned char * pDst, unsigned char * pSrc1, unsigned char * pSrc2, unsigned int width)
;void line_intepolation2(unsigned char * pDst1, unsigned char * pDst2, unsigned char * pSrc1, unsigned char *pSrc2, unsigned int width)
;*************************************************************************************/
 AREA	|.text|, CODE, READONLY


 EXPORT	  line_intepolation2_round
 EXPORT   line_intepolation
 EXPORT	  line_intepolation2
 
 EXPORT		avg_s0
 EXPORT		avg_s1
 EXPORT		avg_s2
 
 EXPORT	  cpy_s1
 EXPORT   cpy_s2
 
 ALIGN 4
 
;void avg_s0(unsigned char * pDst, unsigned char * pSrc1, unsigned char * pSrc2, unsigned int width, unsigned int lstride, unsigned int rstride, unsigned int fill);
avg_s0
	stmfd	sp!,{r4-r7, lr}

	ldr r4, [sp, #20]
	ldr r5, [sp, #28] 
	cmp r4, #0
	beq	avg_s0_fill_l_end
avg_s0_fill_l
	str r5, [r0], #4
	subs  r4, r4, #4
	bne avg_s0_fill_l

avg_s0_fill_l_end	
	mov	r7, #1
	add r7, r7, r7, lsl #8
	add r7, r7, r7, lsl #16
	
	ldr r4, [r1], #4
	ldr r5, [r2], #4
width_loop_round
	uhadd8	r6, r4, r5
	eor		r4, r4, r5
	and		r7, r7, r4
	uadd8	r6, r6, r7
	ldr r4, [r1], #4
	ldr r5, [r2], #4	
	subs r3, r3, #4
	str	r6, [r0], #4
	bne width_loop_round

	ldr r4, [sp, #24]
	ldr r5, [sp, #28] 
	cmp r4, #0
	beq	avg_s0_fill_r_end
avg_s0_fill_r
	str r5, [r0], #4
	subs  r4, r4, #4
	bne avg_s0_fill_r	
avg_s0_fill_r_end
	
	ldmfd   sp!,{r4-r7, pc}


line_intepolation2_round
	stmfd	sp!,{r4-r8, lr}

	mov r8, #1
	add r8, r8, r8, lsl #8
	add r8, r8, r8, lsl #16
	
	ldr r7,[sp, #24]
	ldr r4, [r2], #4
	ldr r5, [r3], #4
width_loop2_round
	uhadd8	r6, r4, r5
	eor		r4, r4, r5
	and		r8, r8, r4
	uadd8	r6, r6, r8
	str r4, [r0], #4
	ldr r4, [r2], #4
	ldr r5, [r3], #4	
	subs r7, r7, #4
	str	r6, [r1], #4
	bne width_loop2_round
	
	ldmfd   sp!,{r4-r8, pc}
	
line_intepolation
	stmfd	sp!,{r4-r7, lr}
	
	ldr r4, [r1], #4
	ldr r5, [r2], #4
width_loop
	uhadd8	r6, r4, r5
	ldr r4, [r1], #4
	ldr r5, [r2], #4	
	subs r3, r3, #4
	str	r6, [r0], #4
	bne width_loop
	ldmfd   sp!,{r4-r7, pc}


line_intepolation2
	stmfd	sp!,{r4-r8, lr}
	
	ldr r7,[sp, #24]
	ldr r4, [r1], #4
	ldr r5, [r2], #4
width_loop2
	uhadd8	r6, r4, r5
	str r4, [r0], #4
	ldr r4, [r2], #4
	ldr r5, [r3], #4	
	subs r7, r7, #4
	str	r6, [r1], #4
	bne width_loop2
	
	ldmfd   sp!,{r4-r8, pc}	
	
;void dither_s1(unsigned char * pDst1, unsigned char * pDst2, unsigned char * pSrc1, unsigned char * pSrc2, unsigned int width, unsigned int dither, unsigned int dither2, unsigned int lstride, unsigned int rstride, unsigned int fill)
;r0 -> pSrc1
;r1 -> pSrc2
;r2 -> pDst1
;r3 -> pDst2
;r12 = sp+40 -> width
;r11 -> 0x01010101
;r9  -> 0x03030303
;r4, r5, r6, r7, r8, r14 is free

avg_s1
	stmfd	sp!,{r4-r12, lr}
	
	ldr r4, [sp, #52]
	ldr r5, [sp, #60] 
	cmp r4, #0
	beq	avg_s1_fill_l_end
avg_s1_fill_l
	str r5, [r0], #4
	str r5, [r1], #4
	subs  r4, r4, #4
	bne avg_s1_fill_l	
avg_s1_fill_l_end

	mov		r11, #1
	add		r11, r11, r11, lsl #8
	add		r11, r11, r11, lsl #16	;r11 = 0x01010101

	ldr		r4, [r2], #4	;top
	ldr		r5, [r3], #4	;bottom
	ldr		r12,[sp, #40]
	ldr		r10,[sp, #44]
	ldr		r14,[sp, #48]
	add		r9, r11, r11, lsl #1	; r9 = 0x03030303
width_loop2_round_dither_3
	uhadd8	r6, r4, r5
	eor		r7, r4, r5
	and		r7, r7, r11
	uadd8	r6, r6, r7
	
	and		r8, r9, r4, lsr #6
	sub		r4, r4, r8
	add		r4, r4, r10
	str		r4, [r0], #4
	
	and     r8, r9, r6, lsr #6
	sub     r8, r6, r8

	ldr		r4, [r2], #4	;top
	ldr		r5, [r3], #4	;bottom
	add		r8, r8, r14
	str		r8, [r1], #4
	
	subs r12, r12, #4
	bne width_loop2_round_dither_3
	
	ldr r4, [sp, #56]
	ldr r5, [sp, #60] 
	cmp r4, #0
	beq	avg_s1_fill_r_end
avg_s1_fill_r
	str r5, [r0], #4
	str r5, [r1], #4
	subs  r4, r4, #4
	bne avg_s1_fill_r	
avg_s1_fill_r_end
	
	ldmfd   sp!,{r4-r12, pc}	
	

;void dither_s2(unsigned char * pDst1, unsigned char * pDst2, unsigned char * pSrc1, unsigned char * pSrc2, unsigned int width, unsigned int dither, unsigned int dither2, unsigned int lstride, unsigned int rstride, unsigned int fill)
;r0 -> pSrc1
;r1 -> pSrc2
;r2 -> pDst1
;r3 -> pDst2
;r12 = sp+40 -> width
;r11 -> 0x01010101
;r9  -> 0x03030303
;r4, r5, r6, r7, r8, r14 is free

avg_s2
	stmfd	sp!,{r4-r12, lr}
	
	ldr r4, [sp, #52]
	ldr r5, [sp, #60] 
	cmp r4, #0
	beq	avg_s2_fill_l_end
avg_s2_fill_l
	str r5, [r0], #4
	str r5, [r1], #4
	subs  r4, r4, #4
	bne avg_s2_fill_l	
avg_s2_fill_l_end
	

	mov		r11, #1
	add		r11, r11, r11, lsl #8
	add		r11, r11, r11, lsl #16	;r11 = 0x01010101

	ldr		r4, [r2], #4	;top
	ldr		r5, [r3], #4	;bottom
	ldr		r12,[sp, #40]
	ldr		r10,[sp, #44]
	ldr		r14,[sp, #48]
	
	mov		r9, r11, lsl #3
	sub		r9, r9, r11				; r9 = 0x07070707
	
width_loop2_round_dither_7
	uhadd8	r6, r4, r5
	eor		r7, r4, r5
	and		r7, r7, r11
	uadd8	r6, r6, r7
	
	and		r8, r9, r4, lsr #5
	sub		r4, r4, r8
	add		r4, r4, r10
	str		r4, [r0], #4
	
	and     r8, r9, r6, lsr #5
	sub     r8, r6, r8

	ldr		r4, [r2], #4	;top
	ldr		r5, [r3], #4	;bottom

	add		r8, r8, r14
	str		r8, [r1], #4
	
	subs r12, r12, #4
	bne width_loop2_round_dither_7
	
	ldr r4, [sp, #56]
	ldr r5, [sp, #60] 
	cmp r4, #0
	beq	avg_s2_fill_r_end
avg_s2_fill_r
	str r5, [r0], #4
	str r5, [r1], #4
	subs  r4, r4, #4
	bne avg_s2_fill_r	
avg_s2_fill_r_end
	
	
	ldmfd   sp!,{r4-r12, pc}		
	
;void cpy_s2(unsigned char * pDst, unsigned char * pSrc, unsigned int width, unsigned int dither, unsigned int lstride, unsigned int rstride, unsigned int fill)
;r0 -> pDst
;r1 -> pSrc
;r2 -> width
;r3 -> dither
;r4 -> 0x01010101
;r5 -> 0x07070707
;r6, r7, r8, r9, r10, r11, r14 is free

cpy_s2
	stmfd	sp!,{r4-r7, lr}
	
	ldr r4, [sp, #20]
	ldr r5, [sp, #28] 
	cmp r4, #0
	beq	cpy_s2_fill_l_end
cpy_s2_fill_l
	str r5, [r0], #4
	subs  r4, r4, #4
	bne cpy_s2_fill_l	
cpy_s2_fill_l_end

	mov		r4, #1
	add		r4, r4, r4, lsl #8
	add		r4, r4, r4, lsl #16	;r4 = 0x01010101

	ldr		r6, [r1], #4
	
	mov		r5, r4, lsl #3
	sub		r5, r5, r4				; r5 = 0x07070707
	
width_loop2_round_cpy_7
	and		r7, r5, r6, lsr #5
	sub		r7, r6, r7
	add		r7, r7, r3
	str		r7, [r0], #4
	ldr		r6, [r1], #4	
	subs r2, r2, #4
	bne width_loop2_round_cpy_7
	
	ldr r4, [sp, #24]
	ldr r5, [sp, #28] 
	cmp r4, #0
	beq	cpy_s2_fill_r_end
cpy_s2_fill_r
	str r5, [r0], #4
	subs  r4, r4, #4
	bne cpy_s2_fill_r	
cpy_s2_fill_r_end
	
	ldmfd   sp!,{r4-r7, pc}		
	
	
;void cpy_s1(unsigned char * pDst, unsigned char * pSrc, unsigned int width, unsigned int dither, unsigned int lstride, unsigned int rstride, unsigned int fill)
;r0 -> pDst
;r1 -> pSrc
;r2 -> width
;r3 -> dither
;r4 -> 0x01010101
;r5 -> 0x03030303
;r6, r7, r8, r9, r10, r11, r14 is free

cpy_s1
	stmfd	sp!,{r4-r7, lr}
	
	ldr r4, [sp, #20]
	ldr r5, [sp, #28] 
	cmp r4, #0
	beq	cpy_s1_fill_l_end
cpy_s1_fill_l
	str r5, [r0], #4
	subs  r4, r4, #4
	bne cpy_s1_fill_l	
cpy_s1_fill_l_end

	mov		r4, #1
	add		r4, r4, r4, lsl #8
	add		r4, r4, r4, lsl #16		;r4 = 0x01010101

	ldr		r6, [r1], #4
	add		r5, r4, r4, lsl #1		;r5 = 0x03030303
	
width_loop2_round_cpy_3
	and		r7, r5, r6, lsr #6
	sub		r7, r6, r7
	add		r7, r7, r3
	str		r7, [r0], #4
	ldr		r6, [r1], #4
	subs r2, r2, #4
	bne width_loop2_round_cpy_3
	
	ldr r4, [sp, #24]
	ldr r5, [sp, #28] 
	cmp r4, #0
	beq	cpy_s1_fill_r_end
cpy_s1_fill_r
	str r5, [r0], #4
	subs  r4, r4, #4
	bne cpy_s2_fill_r	
cpy_s1_fill_r_end
	
	ldmfd   sp!,{r4-r7, pc}			

	END
