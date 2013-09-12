#ifndef __GPINTR_H_
#define __GPINTR_H_

#if defined(INTR_AU13XX_GPINT)
#include <au1x00.h>

#define GPIO_INPUT			GPINT_PINCTL_GPIOINPUT
#define GPIO_LOW			GPINT_PINCTL_GPIOOUT_0
#define GPIO_HIGH			GPINT_PINCTL_GPIOOUT_1
#define GPIO_IS_DEVICE		GPINT_PINCTL_DEVICE
#define CPU_INT0			GPINT_INTLINE_CPUINT_0
#define CPU_INT1			GPINT_INTLINE_CPUINT_1
#define CPU_INT2			GPINT_INTLINE_CPUINT_2
#define CPU_INT3			GPINT_INTLINE_CPUINT_3
#define WAKE_ENABLE			GPINT_INTWAKE_ENABLE
#define IRQ_DISABLED   		GPINT_INTCFG_DISABLE

// default is low priority
#define irqLL				GPINT_INTCFG_LL | GPINT_INTWAKE_ENABLE
#define irqHL				GPINT_INTCFG_HL | GPINT_INTWAKE_ENABLE
#define irqFE				GPINT_INTCFG_FE | GPINT_INTWAKE_ENABLE
#define irqRE				GPINT_INTCFG_RE | GPINT_INTWAKE_ENABLE

#define irqRE_NOWAKE		GPINT_INTCFG_RE

#define irqNA				0

#endif

/*
 *	Platform Configuration structure
 */
typedef struct
{
	int				pin;
	WCHAR			*desc;
	unsigned int    enable;
	unsigned int	cfg;
} PLATFORM_GPINTR_CONFIG, *PPLATFORM_GPINTR_CONFIG;

BOOL GPINTR_SetPinOutputState( ULONG pin, ULONG state );
BOOL GPINTR_SetPinConfiguration( ULONG pin,ULONG cfg );
BOOL GPINTR_GetPinConfiguration( ULONG pin,ULONG *pcfg );
BOOL GPINTR_GetPinState( ULONG pin,ULONG *pstate );
BOOL GPINTR_GetPinResetValue( ULONG pin,ULONG *pval );

#endif //__GPINTR_H_
