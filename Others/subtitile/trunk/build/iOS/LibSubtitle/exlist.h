/*
 * Summary: lists interfaces
 * Description: this module implement the list support used in 
 * various place in the library.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Gary Pennington <Gary.Pennington@uk.sun.com>
 */

/*
modification history:
2007.06.27    leal porting
2007.07.05    leal add custom search function
2007.12.05    leal add new "ExListRemoveNode" function
2008.05.01    leal add mutex control
*/
#ifndef __Ex_LINK_INCLUDE__
#define __Ex_LINK_INCLUDE__


#ifdef __cplusplus
extern "C" {
#endif

struct _ExLink
{
    struct _ExLink *next;
    struct _ExLink *prev;
    void *data;
};

typedef struct _ExLink ExLink;
typedef ExLink *ExLinkPtr;

typedef struct _ExList ExList;
typedef ExList *ExListPtr;

/**
 * ExListDeallocator:
 * @lk:  the data to deallocate
 *
 * Callback function used to free data from a list.
 */
typedef void (*ExListDeallocator) (ExLinkPtr lk);
/**
 * ExListDataCompare:
 * @data0: the first data
 * @data1: the second data
 *
 * Callback function used to compare 2 data.
 *
 * Returns 0 is equality, -1 or 1 otherwise depending on the ordering.
 */
typedef int  (*ExListDataCompare) (const void *data0, const void *data1);
/**
 * ExListWalker:
 * @data: the data found in the list
 * @user: extra user provided data to the walker
 *
 * Callback function used when walking a list with ExListWalk().
 *
 * Returns 0 to stop walking the list, 1 otherwise.
 */
typedef int (*ExListWalker) (const void *data, const void *user);



/* Creation/Deletion */
 ExListPtr ExListCreate	(ExListDeallocator deallocator,
	                                 ExListDataCompare compare);
 void ExListDelete		(ExListPtr l);

/* Basic Operators */
ExLinkPtr ExListLinkSearch(ExListPtr l, void *data) ;
ExLinkPtr ExListLinkReverseSearch(ExListPtr l, void *data) ;

 void * ExListSearch		(ExListPtr l,
					 void *data);
 void * ExListReverseSearch	(ExListPtr l,
					 void *data);
 
ExLinkPtr ExListLinkSearchCustom(ExListPtr l, void *data,ExListDataCompare compare) ;
ExLinkPtr ExListLinkReverseSearchCustom(ExListPtr l, void *data,ExListDataCompare compare) ;

 void * ExListSearchCustom(ExListPtr l, void * data, ExListDataCompare compare);
 void * ExListReverseSearchCustom(ExListPtr l, void * data, ExListDataCompare compare);
    
 ExLinkPtr 	ExListInsert		(ExListPtr l,
					 void *data) ;
 ExLinkPtr 	ExListAppend		(ExListPtr l,
					 void *data) ;
 int 	ExListRemoveFirst	(ExListPtr l,
					 void *data);
 int 	ExListRemoveLast	(ExListPtr l,
					 void *data);
 int 	ExListRemoveAll	(ExListPtr l,
					 void *data);

typedef enum
{
RM_NODE_SELF,
RM_NODE_BACK,
RM_NODE_FRONT,
RM_NODE_SELF_BACK,
RM_NODE_SELF_FRONT,
}eNodeRm;

 /**
 * ExListRemoveNode:
 * @l:  a list
 * @node:  list node for handle
 * @how: how to do about  removing
 *            =="RM_NODE_SELF", only remove the node self
 *            =="RM_NODE_BACK", remove the list nodes back of node(not include node self)
 *            =="RM_NODE_FRONT", remove the list nodes front of node(not include node self)
 *            =="RM_NODE_SELF_BACK", remove the list nodes back of node(include node self)
 *            =="RM_NODE_SELF_FRONT", remove the list nodes front of node(include node self)
 *
 * Remove relate nodes in the list
 *
 * Returns 0 if ok, -1 if error
 */
int
ExListRemoveNode(ExListPtr l, ExLinkPtr node,eNodeRm how);

 int ExListUnlinkNode(ExListPtr l, ExLinkPtr node, int free_data);
 void ExListClear		(ExListPtr l);
 int 	ExListEmpty		(ExListPtr l);
 ExLinkPtr ExListFront		(ExListPtr l);
 ExLinkPtr ExListEnd		(ExListPtr l);
 int 	ExListSize		(ExListPtr l);

 void ExListPopFront		(ExListPtr l);
 void ExListPopBack		(ExListPtr l);
 int 	ExListPushFront	(ExListPtr l,
					 void *data);
 int 	ExListPushBack		(ExListPtr l,
					 void *data);

/* Advanced Operators */
 void ExListReverse		(ExListPtr l);
 void ExListSort		(ExListPtr l);
ExLinkPtr ExListWalk(ExListPtr l, ExListWalker walker, const void *user);

ExLinkPtr ExListReverseWalk(ExListPtr l, ExListWalker walker, const void *user);

 void ExListMerge		(ExListPtr l1,
					 ExListPtr l2);
 ExListPtr ExListDup		(const ExListPtr old);
 int 	ExListCopy		(ExListPtr cur,
					 const ExListPtr old);
/* Link operators */
 void *   ExLinkGetData          (ExLinkPtr lk);

/* ExListUnique() */
/* ExListSwap */

#ifdef __cplusplus
}
#endif

#endif /* __Ex_LINK_INCLUDE__ */

