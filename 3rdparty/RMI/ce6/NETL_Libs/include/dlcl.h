/*
 *  Linked list macros
 *
 * Copyright 2004 Eric DeVolder
 *
 */

#ifndef _DSS_SRC_INCLUDE_DLCL_H
#define _DSS_SRC_INCLUDE_DLCL_H

/********************************************************************/
/*
 * This macro appends ELEMENT to the end of a list
 */
#define DLCL_APPEND(TYPE, QUEUE, ELEMENT)	\
	DLCL_APPEND_NP(TYPE, QUEUE, ELEMENT, next, prev)

/********************************************************************/
/*
 * This macro removes ELEMENT from the list.
 */
#define DLCL_REMOVE(TYPE, QUEUE, ELEMENT)	\
	DLCL_REMOVE_NP(TYPE, QUEUE, ELEMENT, next, prev)

/********************************************************************/
/*
 * This macro extracts ELEMENT from the list. It doesn't
 * update the list head.
 */
#define DLCL_EXTRACT(TYPE, ELEMENT)	\
	DLCL_EXTRACT_NP(TYPE, ELEMENT, next, prev)

/********************************************************************/
/*
 * This macro inserts ELEMENT into the list before NELEMENT
 */
#define DLCL_INSERT_BEFORE(TYPE, ELEMENT, NELEMENT)	\
	DLCL_INSERT_BEFORE_NP(TYPE, ELEMENT, NELEMENT, next, prev)

/********************************************************************/
/*
 * This macro inserts ELEMENT into the list after PELEMENT
 */
#define DLCL_INSERT_AFTER(TYPE, ELEMENT, PELEMENT)	\
	DLCL_INSERT_AFTER_NP(TYPE, ELEMENT, PELEMENT, next, prev)

/********************************************************************/
/*
 * This macro inserts LIST into the list before element NELEMENT.
 */
#define DLCL_INSERT_LIST_BEFORE(TYPE, ELEMENT, NELEMENT)	\
	DLCL_INSERT_LIST_BEFORE_NP(TYPE, LIST, NLIST, next, prev)

/********************************************************************/
/*
 * This macro inserts LIST into the list after element PELEMENT
 */
#define DLCL_INSERT_LIST_AFTER(TYPE, ELEMENT, PELEMENT)	\
	DLCL_INSERT_LIST_AFTER_NP(TYPE, LIST, PLIST, next, prev)

/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/*
 * This macro appends ELEMENT to [the end of] a list
 */
#define DLCL_APPEND_NP(TYPE, QUEUE, ELEMENT, NEXT, PREV)	\
{										\
	TYPE *fel, *lel;				\
	if ((fel = QUEUE) == NULL)			\
	{									\
		QUEUE = ELEMENT->NEXT = ELEMENT->PREV = ELEMENT;	\
	}									\
	else								\
	{									\
		lel = fel->PREV;				\
		lel->NEXT = ELEMENT;			\
		ELEMENT->PREV = lel;			\
		ELEMENT->NEXT = fel;			\
		fel->PREV = ELEMENT;			\
	}									\
}

/********************************************************************/
/*
 * This macro prepends ELEMENT to [the beginning of] a list
 */
#define DLCL_PREPEND_NP(TYPE, QUEUE, ELEMENT, NEXT, PREV)	\
{										\
	TYPE *fel, *lel;				\
	if ((fel = QUEUE) == NULL)			\
	{									\
		QUEUE = ELEMENT->NEXT = ELEMENT->PREV = ELEMENT;	\
	}									\
	else								\
	{									\
		lel = fel->PREV;				\
		lel->NEXT = ELEMENT;			\
		ELEMENT->PREV = lel;			\
		ELEMENT->NEXT = fel;			\
		fel->PREV = ELEMENT;			\
		QUEUE = ELEMENT;	    		\
	}									\
}

/********************************************************************/
/*
 * This macro removes ELEMENT from the list.
 *
 * NOTE: This requires that ELEMENT actually be on the list!
 */
#define DLCL_REMOVE_NP(TYPE, QUEUE, ELEMENT, NEXT, PREV)	\
{										\
	TYPE *nel, *pel;				\
	if (QUEUE != NULL)					\
	{									\
		nel = ELEMENT->NEXT;			\
		pel = ELEMENT->PREV;			\
		if (QUEUE == ELEMENT)			\
		{								\
			/* FIX!!!! should use (nel == pel) */	\
            /*if (nel == pel) */    \
			if (ELEMENT->NEXT == ELEMENT)			\
			{							\
				ELEMENT->NEXT = ELEMENT->PREV = NULL;		\
				QUEUE = NULL;			\
			}							\
			else						\
			{							\
				pel->NEXT = nel;		\
				nel->PREV = pel;		\
				QUEUE = nel;			\
			}							\
		}								\
		else							\
		{								\
			pel->NEXT = nel;			\
			nel->PREV = pel;			\
		}								\
	}									\
}


/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/

/*
 * The following insert and extract macros simply manipulate the
 * element(s) provided. That is, the list head is not modified.
 * Furthermore, it is REQUIRED that the ELEMENT be valid (ie not NULL)
 */

/*
 * This macro extracts ELEMENT from the list. It doesn't
 * change the list head.
 *
 * NOTE: This requires that the element actually be on the list!
 */
#define DLCL_EXTRACT_NP(TYPE, ELEMENT, NEXT, PREV)	\
{									\
	TYPE *nel, *pel;			\
	nel = ELEMENT->NEXT;			\
	pel = ELEMENT->PREV;			\
	pel->NEXT = nel;				\
	nel->PREV = pel;				\
}

/********************************************************************/
/*
 * This macro inserts ELEMENT into the list before NELEMENT.
 */
#define DLCL_INSERT_BEFORE_NP(TYPE, QUEUE, ELEMENT, NELEMENT, NEXT, PREV)	\
{									\
	TYPE *pel;					\
	pel = NELEMENT->PREV;			\
	pel->NEXT = ELEMENT;			\
	ELEMENT->NEXT = NELEMENT;		\
	NELEMENT->PREV = ELEMENT;		\
	ELEMENT->PREV = pel;			\
    if (QUEUE == NELEMENT)          \
        QUEUE = ELEMENT;            \
}

/********************************************************************/
/*
 * This macro inserts ELEMENT into the list after PELEMENT.
 */
#define DLCL_INSERT_AFTER_NP(TYPE, QUEUE, ELEMENT, PELEMENT, NEXT, PREV)	\
{									\
    TYPE *nel;					    \
	nel = PELEMENT->NEXT;			\
	PELEMENT->NEXT = ELEMENT;		\
	ELEMENT->NEXT = nel;			\
	nel->PREV = ELEMENT;			\
	ELEMENT->PREV = PELEMENT; 		\
}

/********************************************************************/
/*
 * This macro inserts ELEMENT into the list in sorted order via EXPR.
 */
#define DLCL_INSERT_SORTED_NP(TYPE, QUEUE, ELEMENT, EXPR, NEXT, PREV)	\
{									                                    \
    TYPE *cel;                                                          \
    if ((cel = QUEUE) == NULL)                                          \
    {                                                                   \
        DLCL_APPEND_NP(TYPE, QUEUE, ELEMENT, NEXT, PREV); /* Start the list */ \
    }                                                                   \
    else                                                                \
    while (cel != NULL)                                                 \
    {                                                                   \
        if (EXPR)                                                       \
        {                                                               \
            DLCL_INSERT_BEFORE_NP(TYPE, QUEUE, ELEMENT, cel, NEXT, PREV); /* Insert in list */ \
            break;                                                      \
        }                                                               \
                                                                        \
        if ((cel = cel->NEXT) == QUEUE)                                 \
        {                                                               \
            DLCL_APPEND_NP(TYPE, QUEUE, ELEMENT, NEXT, PREV); /* Insert at end of list */ \
            cel = NULL;                                                 \
        }                                                               \
    }                                                                   \
}

/*
 * This macro joins two DLCLs together, by concatenating LIST2 at the
 * end of LIST1.
 */
#define DLCL_JOIN_NP(TYPE, QUEUE, LIST1, LIST2, NEXT, PREV) \
{                                                           \
    if (LIST1 && LIST2)                                     \
    {                                                       \
        TYPE *fel1, *fel2, *lel1, *lel2;                    \
        fel1 = LIST1;                                       \
        lel1 = fel1->PREV;                                  \
        fel2 = LIST2;                                       \
        lel2 = fel2->PREV;                                  \
        fel1->PREV = lel2;                                  \
        lel2->NEXT = fel1;                                  \
        lel1->NEXT = fel2;                                  \
        fel2->PREV = lel1;                                  \
        QUEUE = LIST1;                                      \
    }                                                       \
    else                                                    \
    {                                                       \
        if (LIST1)                                          \
            QUEUE = LIST1;                                  \
        else                                                \
            QUEUE = LIST2;                                  \
    }                                                       \
} 

/*
 * This macro splits QUEUE into two DLCLs at element ELEMENT.
 * This requires that ELEMENT be on the QUEUE. When complete,
 * ELEMENT points to a list (as does QUEUE).
 */
#define DLCL_SPLIT_NP(TYPE, QUEUE, ELEMENT, NEXT, PREV) \
{                                                           \
    if (QUEUE && ELEMENT)                                   \
    {                                                       \
        TYPE *fel1, *fel2, *lel1, *lel2;                    \
        fel1 = QUEUE;                                       \
        fel2 = ELEMENT;                                     \
        lel1 = fel2->PREV;                                  \
        lel2 = fel1->PREV;                                  \
        fel1->PREV = lel1;                                  \
        lel1->NEXT = fel1;                                  \
        fel2->PREV = lel2;                                  \
        lel2->NEXT = fel2;                                  \
    }                                                       \
}

/********************************************************************/

/*
 * The following insert macros operate on lists, but do not modify
 * the list head.
 * Furthermore, it is REQUIRED that the LIST be valid (ie not NULL)
 */

/*
 * This macro inserts LIST into the list BEFORE element NELEMENT.
 */
#define DLCL_INSERT_LIST_BEFORE_NP(TYPE, LIST, NELEMENT, NEXT, PREV)	\
{									\
	TYPE *nel, *pel, *lf, *ll;	\
	lf = LIST; ll = lf->PREV;		\
	nel = NELEMENT; pel = nel->PREV;\
	ll->NEXT = nel;					\
	nel->PREV = ll;					\
	pel->NEXT = lf;					\
	lf->PREV = pel;					\
}

/********************************************************************/
/*
 * This macro inserts LIST into the list AFTER element PELEMENT
 */
#define DLCL_INSERT_LIST_AFTER_NP(TYPE, LIST, PELEMENT, NEXT, PREV)	\
{									\
	TYPE *nel, *pel, *lf, *ll;	\
	lf = LIST; ll = lf->PREV;		\
	pel = PELEMENT; nel = pel->NEXT;\
	ll->NEXT = nel;					\
	nel->PREV = ll;					\
	pel->NEXT = lf;					\
	lf->PREV = pel;					\
}

/********************************************************************/
 
#endif	/* _DSS_SRC_INCLUDE_DLCL_H */

