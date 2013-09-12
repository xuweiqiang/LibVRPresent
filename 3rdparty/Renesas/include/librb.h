//********************************************************************
//Renesas ring buffer
//		by Benson Chuang
//			benson.chuang.tw@gmail.com  benson.chuang@renesas.com
//********************************************************************
//Component:	librb
//						Renesas Technology Taiwan/Engineerning Center
//********************************************************************
#ifndef _LIBRB_H
#define _LIBRB_H

//**************************************
//Structure
//**************************************
typedef struct rb_element{
	unsigned long	curr;
	void			*p_data;
	struct rb_element	*p_next;
}RB_ELEMENT;
typedef struct rb{
	unsigned long	total;	
	unsigned long	front;
	unsigned long	rear;
	HANDLE			hHandle;
	RB_ELEMENT		*rb_emt;
}RB;

//**************************************
//Public Function
//**************************************
unsigned long RB_init(RB *head , unsigned long count);
void RB_deinit(RB *head);
unsigned long RB_enqueue(RB *head , void *data);
unsigned long RB_dequeue(RB *head , void *data);
unsigned long RB_setdata(RB *head , RB_ELEMENT *first_element);
unsigned long RB_isfull(RB *head);


//**************************************
//Private Function
//**************************************

//**************************************
//Behavior setting
//**************************************


//**************************************
//Desplayint type
//**************************************

//**************************************
//IOCTL command
//**************************************

//**************************************
//Error handling
//**************************************
enum RB_ERROR{
	RB_OK = 0,
	RB_FAIL,
	RB_EMPTY,
	RB_FULL,
};
#endif