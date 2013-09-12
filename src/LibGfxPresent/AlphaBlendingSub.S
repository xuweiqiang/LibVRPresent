
	
	.global DoAlphaBlending1ByteSub_WMMX
	
    @AREA	WMMX, CODE, READONLY
    
DoAlphaBlending1ByteSub_WMMX:
		ldr				r4,			[r2, #0]				@ load alpha1
		ldr				r5,			[r2, #4]				@ load alpha2
		tbcsth			wr14,		r4						@
		tbcsth			wr15,		r5						@
		
		ldr				r4,			[r2, #12]				@ load background color
		tbcstb			wr13,		r4						@		


		ldr				r6,			[r2, #8]				@ load s32VisibleHeight
ALPHA_LOOP_H:
		mov				r8,			r0						@ preserve the pointer
		mov				r9,			r1						@
		
		mov				r4,			r0						@
		and				r4,			r4,			#7			@
		mov				r5,			r1						@
		and				r5,			r5,			#7			@
		bic				r0,			r0,			#7			@ align pu8Image1 to 8 bytes
		bic				r1,			r1,			#7			@ align pu8Image2 to 8 bytes
		tmcr			wcgr0,		r4						@
		tmcr			wcgr1,		r5						@
		
		rsb				r5,			r4,			#8			@ calculate shift bits for previous result
		mov				r5,			r5,	lsl #3				@
		tmcr			wcgr3,		r5						@
		mov				r4,			r4, lsl #3				@ calculate shift bits for current result
		tmcr			wcgr2,		r4						@
		
		
		wldrd			wr0,		[r0, #0]				@ pre load image1 data
		wldrd			wr2,		[r1, #0]				@ pre load image2 data
		wldrd			wr1,		[r0, #8]				@
		wldrd			wr3,		[r1, #8]				@
		wslldg			wr8,		wr0,		wcgr3		@
		
		ldr				r7,			[r2, #16]				@ load u32VisibleWidthIn16
		cmp				r7,			#0						@
		beq				ALPHA_REMAINPIXEL16					@
ALPHA_LOOP_W:
		walignr0		wr4,		wr0,		wr1			@
		walignr1		wr5,		wr2,		wr3			@
		
		wcmpeqb			wr6,		wr5,		wr13		@ compare with background color
		wmov			wr7,		wr4						@
		wand			wr7,		wr7,		wr6			@
		wandn			wr5,		wr5,		wr6			@
		wor				wr5,		wr5,		wr7			@
		
		wunpckehub		wr6,		wr4						@ unpack image1 data, wr6 = high1
		wunpckehub		wr7,		wr5						@ unpack image2 data, wr7 = high2
		wunpckelub		wr4,		wr4						@ unpack image1 data, wr4 = low1
		wunpckelub		wr5,		wr5						@ unpack image2 data, wr5 = low2
		
		wmulum			wr4,		wr4,		wr14		@ image1 * alpha1
		wmulum			wr6,		wr6,		wr14		@
		wmulum			wr5,		wr5,		wr15		@ image2 * alpha2
		wmulum			wr7,		wr7,		wr15		@
		
		wldrd			wr0,		[r0, #16]				@ pre load next 8 image1 data
		wldrd			wr2,		[r1, #16]				@ pre load next 8 image2 data
		
		wpackhus		wr4,		wr4,		wr6			@ pack image1 data, wr4 = image1
		wpackhus		wr5,		wr5,		wr7			@ pack image2 data, wr5 = image2
		
		waddbus			wr4,		wr4,		wr5			@ wr4 = image1 + image2
		
		wsrldg			wr8,		wr8,		wcgr3		@
		wmov			wr9,		wr4						@
		wslldg			wr4,		wr4,		wcgr2		@
		wor				wr4,		wr4,		wr8			@
		wstrd			wr4,		[r0, #0]				@ save the result
		
		@ next 8 pixels begins
		walignr0		wr4,		wr1,		wr0			@
		walignr1		wr5,		wr3,		wr2			@

		wcmpeqb			wr6,		wr5,		wr13		@ compare with background color
		wmov			wr7,		wr4						@
		wand			wr7,		wr7,		wr6			@
		wandn			wr5,		wr5,		wr6			@
		wor				wr5,		wr5,		wr7			@
		
		wunpckehub		wr6,		wr4						@ unpack image1 data, wr6 = high1
		wunpckehub		wr7,		wr5						@ unpack image2 data, wr7 = high2
		wunpckelub		wr4,		wr4						@ unpack image1 data, wr4 = low1
		wunpckelub		wr5,		wr5						@ unpack image2 data, wr5 = low2
		
		wmulum			wr4,		wr4,		wr14		@ image1 * alpha1
		wmulum			wr6,		wr6,		wr14		@
		wmulum			wr5,		wr5,		wr15		@ image2 * alpha2
		wmulum			wr7,		wr7,		wr15		@
		
		wldrd			wr1,		[r0, #24]				@ pre load next 8 image1 data
		wldrd			wr3,		[r1, #24]				@ pre load next 8 image2 data
		
		wpackhus		wr4,		wr4,		wr6			@ pack image1 data, wr4 = image1
		wpackhus		wr5,		wr5,		wr7			@ pack image2 data, wr5 = image2
		
		waddbus			wr4,		wr4,		wr5			@ wr4 = image1 + image2
		
		wsrldg			wr9,		wr9,		wcgr3		@
		wmov			wr8,		wr4						@
		wslldg			wr4,		wr4,		wcgr2		@
		wor				wr4,		wr4,		wr9			@
		wstrd			wr4,		[r0, #8]				@ save the result
		
		@ pointer increase & loop counter decrease
		add				r0,			r0,			#16			@
		add				r1,			r1,			#16			@
		subs			r7,			r7,			#1			@
		bne				ALPHA_LOOP_W						@
@ ALPHA_LOOP_W  end

ALPHA_REMAINPIXEL16:
		ldr				r4,			[r2, #20]				@
		cmp				r4,			#8						@
		ble				ALPHA_REMAINPIXEL8					@
		
		walignr0		wr4,		wr0,		wr1			@
		walignr1		wr5,		wr2,		wr3			@
		
		wcmpeqb			wr6,		wr5,		wr13		@ compare with background color
		wmov			wr7,		wr4						@
		wand			wr7,		wr7,		wr6			@
		wandn			wr5,		wr5,		wr6			@
		wor				wr5,		wr5,		wr7			@
		
		wunpckehub		wr6,		wr4						@ unpack image1 data, wr6 = high1
		wunpckehub		wr7,		wr5						@ unpack image2 data, wr7 = high2
		wunpckelub		wr4,		wr4						@ unpack image1 data, wr4 = low1
		wunpckelub		wr5,		wr5						@ unpack image2 data, wr5 = low2
		
		wmulum			wr4,		wr4,		wr14		@ image1 * alpha1
		wmulum			wr6,		wr6,		wr14		@
		wmulum			wr5,		wr5,		wr15		@ image2 * alpha2
		wmulum			wr7,		wr7,		wr15		@
		
		wldrd			wr0,		[r0, #16]				@ pre load next 8 image1 data
		wldrd			wr2,		[r1, #16]				@ pre load next 8 image2 data
		
		wpackhus		wr4,		wr4,		wr6			@ pack image1 data, wr4 = image1
		wpackhus		wr5,		wr5,		wr7			@ pack image2 data, wr5 = image2
		
		waddbus			wr4,		wr4,		wr5			@ wr4 = image1 + image2
		
		wsrldg			wr8,		wr8,		wcgr3		@
		wmov			wr9,		wr4						@
		wslldg			wr4,		wr4,		wcgr2		@
		wor				wr4,		wr4,		wr8			@
		wstrd			wr4,		[r0, #0]				@ save the result
		
		@ next 8 pixels begins
		walignr0		wr4,		wr1,		wr0			@
		walignr1		wr5,		wr3,		wr2			@

		wcmpeqb			wr6,		wr5,		wr13		@ compare with background color
		wmov			wr7,		wr4						@
		wand			wr7,		wr7,		wr6			@
		wandn			wr5,		wr5,		wr6			@
		wor				wr5,		wr5,		wr7			@
		
		wunpckehub		wr6,		wr4						@ unpack image1 data, wr6 = high1
		wunpckehub		wr7,		wr5						@ unpack image2 data, wr7 = high2
		wunpckelub		wr4,		wr4						@ unpack image1 data, wr4 = low1
		wunpckelub		wr5,		wr5						@ unpack image2 data, wr5 = low2
		
		wmulum			wr4,		wr4,		wr14		@ image1 * alpha1
		wmulum			wr6,		wr6,		wr14		@
		wmulum			wr5,		wr5,		wr15		@ image2 * alpha2
		wmulum			wr7,		wr7,		wr15		@
		
		wpackhus		wr4,		wr4,		wr6			@ pack image1 data, wr4 = image1
		wpackhus		wr5,		wr5,		wr7			@ pack image2 data, wr5 = image2
		
		waddbus			wr4,		wr4,		wr5			@ wr4 = image1 + image2
		
		wsrldg			wr9,		wr9,		wcgr3		@
		wmov			wr8,		wr4						@
		wslldg			wr4,		wr4,		wcgr2		@
		wor				wr4,		wr4,		wr9			@
		
		and				r4,			r4,			#7			@ save remained pixels
		tmrc			r5,			wcgr3					@
		mov				r5,			r5, lsr	#3				@
		cmp				r5,			r4						@
		blt				ALPHA_BRANCH_1						@

		sub				r4,			r5,			r4			@
		rsb				r5,			r4,			#8			@
		mov				r4,			r4,	lsl #3				@
		tmcr			wcgr3,		r4						@
		mov				r5,			r5, lsl #3				@
		tmcr			wcgr2,		r5						@
		wslldg			wr4,		wr4,		wcgr3		@
		wsrldg			wr4,		wr4,		wcgr3		@
		wsrldg			wr1,		wr1,		wcgr2		@
		wslldg			wr1,		wr1,		wcgr2		@
		wor				wr4,		wr4,		wr1			@
		wstrd			wr4,		[r0, #8]				@
		b				ALPHA_LINE_DONE						@

ALPHA_BRANCH_1:
		wstrd			wr4,		[r0, #8]				@
		wsrldg			wr8,		wr8,		wcgr3		@
		sub				r4,			r4,			r5			@
		rsb				r5,			r4,			#8			@
		mov				r4,			r4, lsl #3				@
		tmcr			wcgr3,		r4						@
		mov				r5,			r5, lsl #3				@
		tmcr			wcgr2,		r5						@
		wslldg			wr8,		wr8,		wcgr2		@
		wsrldg			wr8,		wr8,		wcgr2		@
		wsrldg			wr0,		wr0,		wcgr3		@
		wslldg			wr0,		wr0,		wcgr3		@
		wor				wr8,		wr8,		wr0			@
		wstrd			wr8,		[r0, #16]				@
		b				ALPHA_LINE_DONE						@
		
ALPHA_REMAINPIXEL8:
		cmp				r4,			#0						@
		beq				ALPHA_REMAINPIXEL0					@
		
		walignr0		wr4,		wr0,		wr1			@
		walignr1		wr5,		wr2,		wr3			@
		
		wcmpeqb			wr6,		wr5,		wr13		@ compare with background color
		wmov			wr7,		wr4						@
		wand			wr7,		wr7,		wr6			@
		wandn			wr5,		wr5,		wr6			@
		wor				wr5,		wr5,		wr7			@
		
		wunpckehub		wr6,		wr4						@ unpack image1 data, wr6 = high1
		wunpckehub		wr7,		wr5						@ unpack image2 data, wr7 = high2
		wunpckelub		wr4,		wr4						@ unpack image1 data, wr4 = low1
		wunpckelub		wr5,		wr5						@ unpack image2 data, wr5 = low2
		
		wmulum			wr4,		wr4,		wr14		@ image1 * alpha1
		wmulum			wr6,		wr6,		wr14		@
		wmulum			wr5,		wr5,		wr15		@ image2 * alpha2
		wmulum			wr7,		wr7,		wr15		@
		
		wpackhus		wr4,		wr4,		wr6			@ pack image1 data, wr4 = image1
		wpackhus		wr5,		wr5,		wr7			@ pack image2 data, wr5 = image2
		
		waddbus			wr4,		wr4,		wr5			@ wr4 = image1 + image2
		
		wsrldg			wr8,		wr8,		wcgr3		@
		wmov			wr9,		wr4						@
		wslldg			wr4,		wr4,		wcgr2		@
		wor				wr4,		wr4,		wr8			@

		tmrc			r5,			wcgr3					@ save remained pixels
		mov				r5,			r5, lsr	#3				@
		cmp				r5,			r4						@
		blt				ALPHA_BRANCH_2						@

		sub				r4,			r5,			r4			@
		rsb				r5,			r4,			#8			@
		mov				r4,			r4,	lsl #3				@
		tmcr			wcgr3,		r4						@
		mov				r5,			r5, lsl #3				@
		tmcr			wcgr2,		r5						@
		wslldg			wr4,		wr4,		wcgr3		@
		wsrldg			wr4,		wr4,		wcgr3		@
		wsrldg			wr0,		wr0,		wcgr2		@
		wslldg			wr0,		wr0,		wcgr2		@
		wor				wr4,		wr4,		wr0			@
		wstrd			wr4,		[r0, #0]				@
		b				ALPHA_LINE_DONE						@

ALPHA_BRANCH_2:
		wstrd			wr4,		[r0, #0]				@
		wsrldg			wr9,		wr9,		wcgr3		@
		sub				r4,			r4,			r5			@
		rsb				r5,			r4,			#8			@
		mov				r4,			r4, lsl #3				@
		tmcr			wcgr3,		r4						@
		mov				r5,			r5, lsl #3				@
		tmcr			wcgr2,		r5						@
		wslldg			wr9,		wr9,		wcgr2		@
		wsrldg			wr9,		wr9,		wcgr2		@
		wsrldg			wr1,		wr1,		wcgr3		@
		wslldg			wr1,		wr1,		wcgr3		@
		wor				wr9,		wr9,		wr1			@
		wstrd			wr9,		[r0, #8]				@
		b				ALPHA_LINE_DONE						@

ALPHA_REMAINPIXEL0:
		wsrldg			wr8,		wr8,		wcgr3		@ save remained pixels
		wsrldg			wr0,		wr0,		wcgr2		@
		wslldg			wr0,		wr0,		wcgr2		@
		wor				wr8,		wr8,		wr0			@
		wstrd			wr8,		[r0, #0]				@

ALPHA_LINE_DONE:
		@ pointer increase & loop counter decrease
		ldr				r4,			[r2, #24]				@
		ldr				r5,			[r2, #28]				@
		mov				r0,			r8						@
		mov				r1,			r9						@
		add				r0,			r0,			r4			@
		add				r1,			r1,			r5			@
		subs			r6,			r6,			#1			@
		bne				ALPHA_LOOP_H						@
@ ALPHA_LOOP_H  end

		mov				pc,			lr						@

				@END
