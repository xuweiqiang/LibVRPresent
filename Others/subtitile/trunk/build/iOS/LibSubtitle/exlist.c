
/*
 * list.c: lists handling implementation
 *
 * Copyright (C) 2000 Gary Pennington and Daniel Veillard.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE AUTHORS AND
 * CONTRIBUTORS ACCEPT NO RESPONSIBILITY IN ANY CONCEIVABLE MANNER.
 *
 * Author: Gary.Pennington@uk.sun.com
 */

/*
modification history:
2007.06.27    leal porting
2007.07.05    leal add custom search function
2007.12.05    leal add new "ExListRemoveNode" function
2008.05.01    leal add mutex control
*/

//#define USE_LIST_MUTEX

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef USE_LIST_MUTEX
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#else
#include <windows.h>
#endif
#endif

#ifdef MEMWATCH
#include "memwatch.h"
#endif

#include "ExList.h"

/*
 * Type definition are kept internal
 */

struct _ExList
{
    ExLinkPtr sentinel;
    void (*linkDeallocator)(ExLinkPtr );
    int (*linkCompare)(const void *, const void*);
#ifdef USE_LIST_MUTEX
#ifdef HAVE_PTHREAD_H
   pthread_mutex_t mutex;
#else
   CRITICAL_SECTION mutex;
#endif
#endif
};


/************************************************************************
 *                                    *
 *                Interfaces                *
 *                                    *
 ************************************************************************/

/**
 * ExLinkDeallocator:
 * @l:  a list
 * @lk:  a link
 *
 * Unlink and deallocate @lk from list @l
 */
static void
ExLinkDeallocator(ExListPtr l, ExLinkPtr lk)
{
#ifdef USE_LIST_MUTEX
#ifdef HAVE_PTHREAD_H
    	pthread_mutex_lock(&(l->mutex));
#else
        EnterCriticalSection(&(l->mutex));
#endif
#endif
    (lk->prev)->next = lk->next;
    (lk->next)->prev = lk->prev;
#ifdef USE_LIST_MUTEX
#ifdef HAVE_PTHREAD_H
    	pthread_mutex_unlock(&(l->mutex));
#else
        LeaveCriticalSection(&(l->mutex));
#endif
#endif    
    if(l->linkDeallocator)
        l->linkDeallocator(lk);
    free(lk);
}

/**
 * ExLinkCompare:
 * @data0:  first data
 * @data1:  second data
 *
 * Compares two arbitrary data
 *
 * Returns -1, 0 or 1 depending on whether data1 is greater equal or smaller
 *          than data0
 */
static int
ExLinkCompare(const void *data0, const void *data1)
{
    if (data0 < data1)
        return (-1);
    else if (data0 == data1)
	return (0);
    return (1);
}

/**
 * ExListLowerSearch:
 * @l:  a list
 * @data:  a data
 *
 * Search data in the ordered list walking from the beginning
 *
 * Returns the link containing the data or NULL
 */
static ExLinkPtr 
ExListLowerSearch(ExListPtr l, void *data) 
{
    ExLinkPtr lk;

    if (l == NULL)
        return(NULL);
    for(lk = l->sentinel->next;lk != l->sentinel && l->linkCompare(lk->data, data) <0 ;lk = lk->next);
    return lk;    
}

/**
 * ExListLowerSearch:
 * @l:  a list
 * @data:  a data
 * @compare: custom function
 *
 * Search data in the ordered list walking from the beginning
 *
 * Returns the link containing the data or NULL
 */
static ExLinkPtr 
ExListLowerSearchCustom(ExListPtr l, void *data,ExListDataCompare compare) 
{
    ExLinkPtr lk;
    ExListDataCompare usecompare;
    if (l == NULL)
        return(NULL);
    usecompare=((compare) ? compare : (l->linkCompare)) ;
    for(lk = l->sentinel->next;lk != l->sentinel && usecompare(lk->data, data) <0 ;lk = lk->next);
    return lk;    
}

/**
 * ExListHigherSearch:
 * @l:  a list
 * @data:  a data
 *
 * Search data in the ordered list walking backward from the end
 *
 * Returns the link containing the data or NULL
 */
static ExLinkPtr 
ExListHigherSearch(ExListPtr l, void *data) 
{
    ExLinkPtr lk;

    if (l == NULL)
        return(NULL);
    for(lk = l->sentinel->prev;lk != l->sentinel && l->linkCompare(lk->data, data) >0 ;lk = lk->prev);
    return lk;    
}

/**
 * ExListHigherSearch:
 * @l:  a list
 * @data:  a data
 * @compare: custom function
 *
 * Search data in the ordered list walking backward from the end
 *
 * Returns the link containing the data or NULL
 */
static ExLinkPtr 
ExListHigherSearchCustom(ExListPtr l, void *data,ExListDataCompare compare) 
{
    ExLinkPtr lk;
    ExListDataCompare usecompare;
    if (l == NULL)
        return(NULL);
    usecompare=((compare) ? compare : (l->linkCompare)) ;
    for(lk = l->sentinel->prev;lk != l->sentinel && usecompare(lk->data, data) >0 ;lk = lk->prev);
    return lk;    
}

/**
 * ExListSearch:
 * @l:  a list
 * @data:  a data
 *
 * Search data in the list
 *
 * Returns the link containing the data or NULL
 */
ExLinkPtr 
ExListLinkSearch(ExListPtr l, void *data) 
{
    ExLinkPtr lk;
    if (l == NULL)
        return(NULL);
    lk = ExListLowerSearch(l, data);
    if (lk == l->sentinel)
        return NULL;
    else {
        if (l->linkCompare(lk->data, data) ==0)
            return lk;
        return NULL;
    }
}

/**
 * ExListSearch:
 * @l:  a list
 * @data:  a data
 * @compare: custom function
 *
 * Search data in the list
 *
 * Returns the link containing the data or NULL
 */
ExLinkPtr 
ExListLinkSearchCustom(ExListPtr l, void *data,ExListDataCompare compare) 
{
    ExLinkPtr lk;
    ExListDataCompare usecompare;
    if (l == NULL)
        return(NULL);
    usecompare=((compare) ? compare : (l->linkCompare)) ;
    lk = ExListLowerSearchCustom(l, data,compare);
    if (lk == l->sentinel)
        return NULL;
    else {
        if (usecompare(lk->data, data) ==0)
            return lk;
        return NULL;
    }
}

/**
 * ExListLinkReverseSearch:
 * @l:  a list
 * @data:  a data
 *
 * Search data in the list processing backward
 *
 * Returns the link containing the data or NULL
 */
ExLinkPtr 
ExListLinkReverseSearch(ExListPtr l, void *data) 
{
    ExLinkPtr lk;
    if (l == NULL)
        return(NULL);
    lk = ExListHigherSearch(l, data);
    if (lk == l->sentinel)
        return NULL;
    else {
        if (l->linkCompare(lk->data, data) ==0)
            return lk;
        return NULL;
    }
}

/**
 * ExListLinkReverseSearch:
 * @l:  a list
 * @data:  a data
 * @compare: custom function
 *
 * Search data in the list processing backward
 *
 * Returns the link containing the data or NULL
 */
ExLinkPtr 
ExListLinkReverseSearchCustom(ExListPtr l, void *data,ExListDataCompare compare) 
{
    ExLinkPtr lk;
    ExListDataCompare usecompare;
    if (l == NULL)
        return(NULL);
    usecompare=((compare) ? compare : (l->linkCompare)) ;
    lk = ExListHigherSearchCustom(l, data,compare);
    if (lk == l->sentinel)
        return NULL;
    else {
        if (usecompare(lk->data, data) ==0)
            return lk;
        return NULL;
    }
}

/**
 * ExListCreate:
 * @deallocator:  an optional deallocator function
 * @compare:  an optional comparison function
 *
 * Create a new list
 *
 * Returns the new list or NULL in case of error
 */
ExListPtr
ExListCreate(ExListDeallocator deallocator, ExListDataCompare compare)
{
    ExListPtr l;
    if (NULL == (l = (ExListPtr )malloc( sizeof(ExList)))) {
        printf("Cannot initialize memory for list");
        return (NULL);
    }
    /* Initialize the list to NULL */
    memset(l, 0, sizeof(ExList));
    
    /* Add the sentinel */
    if (NULL ==(l->sentinel = (ExLinkPtr )malloc(sizeof(ExLink)))) {
        printf("Cannot initialize memory for sentinel");		        
	free(l);
        return (NULL);
    }
    l->sentinel->next = l->sentinel;
    l->sentinel->prev = l->sentinel;
    l->sentinel->data = NULL;
    
    /* If there is a link deallocator, use it */
    if (deallocator != NULL)
        l->linkDeallocator = deallocator;
    /* If there is a link comparator, use it */
    if (compare != NULL)
        l->linkCompare = compare;
    else /* Use our own */
        l->linkCompare = ExLinkCompare;
#ifdef USE_LIST_MUTEX
#ifdef HAVE_PTHREAD_H
    	pthread_mutex_init(&(l->mutex), NULL);
#else
        InitializeCriticalSection(&(l->mutex));
#endif
#endif
    return l;
}
    
/**
 * ExListSearch:
 * @l:  a list
 * @data:  a search value
 *
 * Search the list for an existing value of @data
 *
 * Returns the value associated to @data or NULL in case of error
 */
void *
ExListSearch(ExListPtr l, void *data) 
{
    ExLinkPtr lk;
    if (l == NULL)
        return(NULL);
    lk = ExListLinkSearch(l, data);
    if (lk)
        return (lk->data);
    return NULL;
}

/**
 * ExListSearch:
 * @l:  a list
 * @data:  a search value
 * @compare: custom function
 *
 * Search the list for an existing value of @data
 *
 * Returns the value associated to @data or NULL in case of error
 */
void *
ExListSearchCustom(ExListPtr l, void *data,ExListDataCompare compare) 
{
    ExLinkPtr lk;
    if (l == NULL)
        return(NULL);
    lk = ExListLinkSearchCustom(l, data,compare);
    if (lk)
        return (lk->data);
    return NULL;
}


/**
 * ExListReverseSearch:
 * @l:  a list
 * @data:  a search value
 *
 * Search the list in reverse order for an existing value of @data
 *
 * Returns the value associated to @data or NULL in case of error
 */
void *
ExListReverseSearch(ExListPtr l, void *data) 
{
    ExLinkPtr lk;
    if (l == NULL)
        return(NULL);
    lk = ExListLinkReverseSearch(l, data);
    if (lk)
        return (lk->data);
    return NULL;
}

/**
 * ExListReverseSearch:
 * @l:  a list
 * @data:  a search value
 * @compare: custom function
 *
 * Search the list in reverse order for an existing value of @data
 *
 * Returns the value associated to @data or NULL in case of error
 */
void *
ExListReverseSearchCustom(ExListPtr l, void *data,ExListDataCompare compare) 
{
    ExLinkPtr lk;
    if (l == NULL)
        return(NULL);
    lk = ExListLinkReverseSearchCustom(l, data,compare);
    if (lk)
        return (lk->data);
    return NULL;
}

/**
 * ExListInsert:
 * @l:  a list
 * @data:  the data
 *
 * Insert data in the ordered list at the beginning for this value
 *
 * Returns !NULL in case of success, NULL in case of failure
 */
ExLinkPtr
ExListInsert(ExListPtr l, void *data) 
{
    ExLinkPtr lkPlace, lkNew;

    if (l == NULL)
        return(NULL);
    lkPlace = ExListLowerSearch(l, data);
    /* Add the new link */
    lkNew = (ExLinkPtr) malloc(sizeof(ExLink));
    if (lkNew == NULL) {
        printf("Cannot initialize memory for new link");
        return (NULL);
    }
    lkNew->data = data;
    lkPlace = lkPlace->prev;
    lkNew->next = lkPlace->next;
    (lkPlace->next)->prev = lkNew;
    lkPlace->next = lkNew;
    lkNew->prev = lkPlace;
    return lkNew;
}

/**
 * ExListAppend:
 * @l:  a list
 * @data:  the data
 *
 * Insert data in the ordered list at the end for this value
 *
 * Returns !NULL in case of success, NULL in case of failure
 */
ExLinkPtr ExListAppend(ExListPtr l, void *data) 
{
    ExLinkPtr lkPlace, lkNew;

    if (l == NULL)
        return(NULL);
    lkPlace = ExListHigherSearch(l, data);
    /* Add the new link */
    lkNew = (ExLinkPtr) malloc(sizeof(ExLink));
    if (lkNew == NULL) {
        printf("Cannot initialize memory for new link");
        return (NULL);
    }
#ifdef USE_LIST_MUTEX
#ifdef HAVE_PTHREAD_H
    	pthread_mutex_lock(&(l->mutex));
#else
        EnterCriticalSection(&(l->mutex));
#endif
#endif
    lkNew->data = data;
    lkNew->next = lkPlace->next;
    (lkPlace->next)->prev = lkNew;
    lkPlace->next = lkNew;
    lkNew->prev = lkPlace;
#ifdef USE_LIST_MUTEX
#ifdef HAVE_PTHREAD_H
    	pthread_mutex_unlock(&(l->mutex));
#else
        LeaveCriticalSection(&(l->mutex));
#endif
#endif
    return lkNew;
}

/**
 * ExListDelete:
 * @l:  a list
 *
 * Deletes the list and its associated data
 */
void ExListDelete(ExListPtr l)
{
    if (l == NULL)
        return;

    ExListClear(l);
    free(l->sentinel);
#ifdef USE_LIST_MUTEX
#ifdef HAVE_PTHREAD_H
    	pthread_mutex_destroy(&(l->mutex));
#else
        DeleteCriticalSection(&(l->mutex));
#endif
#endif
    free(l);
}

/**
 * ExListRemoveFirst:
 * @l:  a list
 * @data:  list data
 *
 * Remove the first instance associated to data in the list
 *
 * Returns 1 if a deallocation occured, or 0 if not found
 */
int
ExListRemoveFirst(ExListPtr l, void *data)
{
    ExLinkPtr lk;
    
    if (l == NULL)
        return(0);
    /*Find the first instance of this data */
    lk = ExListLinkSearch(l, data);
    if (lk != NULL) {
        ExLinkDeallocator(l, lk);
        return 1;
    }
    return 0;
}

/**
 * ExListRemoveLast:
 * @l:  a list
 * @data:  list data
 *
 * Remove the last instance associated to data in the list
 *
 * Returns 1 if a deallocation occured, or 0 if not found
 */
int
ExListRemoveLast(ExListPtr l, void *data)
{
    ExLinkPtr lk;
    
    if (l == NULL)
        return(0);
    /*Find the last instance of this data */
    lk = ExListLinkReverseSearch(l, data);
    if (lk != NULL) {
	ExLinkDeallocator(l, lk);
        return 1;
    }
    return 0;
}

/**
 * ExListRemoveAll:
 * @l:  a list
 * @data:  list data
 *
 * Remove the all instance associated to data in the list
 *
 * Returns the number of deallocation, or 0 if not found
 */
int
ExListRemoveAll(ExListPtr l, void *data)
{
    int count=0;
    
    if (l == NULL)
        return(0);

    while(ExListRemoveFirst(l, data))
        count++;
    return count;
}


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
ExListRemoveNode(ExListPtr l, ExLinkPtr node,eNodeRm how)
{
    ExLinkPtr lk=node;
    ExLinkPtr next =NULL;
    ExLinkPtr prev =NULL;
    
    if (l == NULL || node == NULL)
        return(-1);
    switch(how){
       case RM_NODE_SELF:
               ExLinkDeallocator(l, lk);
               break;
       case RM_NODE_BACK:  
               lk=node->next;
               while(lk != l->sentinel) {
                    next = lk->next;
                    ExLinkDeallocator(l, lk);
                    lk = next;
                }
                break;
       case RM_NODE_FRONT:
                lk=node->prev;
                while(lk != l->sentinel) {
                    prev = lk->prev;
                    ExLinkDeallocator(l, lk);
                    lk = prev;
                }
                break;               
       case RM_NODE_SELF_BACK:
               while(lk != l->sentinel) {
                    next = lk->next;
                    ExLinkDeallocator(l, lk);
                    lk = next;
                }
                break;
       case RM_NODE_SELF_FRONT:
               while(lk != l->sentinel) {
                    prev = lk->prev;
                    ExLinkDeallocator(l, lk);
                    lk = prev;
                }
                break;
       default :
                return (-1);
    }
    return 0;
}


/**
 * ExLinkUn:
 * @l:  a list
 * @lk:  a link
 *
 * Unlink  @lk from list @l
 */
static void
ExLinkUn(ExListPtr l, ExLinkPtr lk)
{
#ifdef USE_LIST_MUTEX
#ifdef HAVE_PTHREAD_H
    	pthread_mutex_lock(&(l->mutex));
#else
        EnterCriticalSection(&(l->mutex));
#endif
#endif
    (lk->prev)->next = lk->next;
    (lk->next)->prev = lk->prev;
#ifdef USE_LIST_MUTEX
#ifdef HAVE_PTHREAD_H
    	pthread_mutex_unlock(&(l->mutex));
#else
        LeaveCriticalSection(&(l->mutex));
#endif
#endif    
     free(lk);
}


/**
 * ExListUnlinkNode:
 * @l:  a list
 * @node:  list node for handle
 * @free_data: ==1 unlink this node and free it's data
 *             ==0 only unlink this node
 *
 *
 * Remove relate nodes in the list
 *
 * Returns 0 if ok, -1 if err
 *           
 */
int
ExListUnlinkNode(ExListPtr l, ExLinkPtr node, int free_data)
{
    void * data=node->data;
    ExLinkPtr lk=node;
   
    if (l == NULL || node == NULL)
        return(-1);
    if(free_data){
        ExLinkDeallocator(l, lk);
    }
    else{
        ExLinkUn(l, lk);
    }
    return 0;
}



/**
 * ExListClear:
 * @l:  a list
 *
 * Remove the all data in the list
 */
void
ExListClear(ExListPtr l)
{
    ExLinkPtr  lk;
    
    if (l == NULL)
        return;
    lk = l->sentinel->next;
    while(lk != l->sentinel) {
        ExLinkPtr next = lk->next;

        ExLinkDeallocator(l, lk);
        lk = next;
    }
}

/**
 * ExListEmpty:
 * @l:  a list
 *
 * Is the list empty ?
 *
 * Returns 1 if the list is empty, 0 if not empty and -1 in case of error
 */
int
ExListEmpty(ExListPtr l)
{
    if (l == NULL)
        return(-1);
    return (l->sentinel->next == l->sentinel);
}

/**
 * ExListFront:
 * @l:  a list
 *
 * Get the first element in the list
 *
 * Returns the first element in the list, or NULL
 */
ExLinkPtr 
ExListFront(ExListPtr l)
{
    if (l == NULL)
        return(NULL);
    return (l->sentinel->next);
}
    
/**
 * ExListEnd:
 * @l:  a list
 *
 * Get the last element in the list
 *
 * Returns the last element in the list, or NULL
 */
ExLinkPtr 
ExListEnd(ExListPtr l)
{
    if (l == NULL)
        return(NULL);
    return (l->sentinel->prev);
}
    
/**
 * ExListSize:
 * @l:  a list
 *
 * Get the number of elements in the list
 *
 * Returns the number of elements in the list or -1 in case of error
 */
int
ExListSize(ExListPtr l)
{
    ExLinkPtr lk;
    int count=0;

    if (l == NULL)
        return(-1);
#ifdef USE_LIST_MUTEX
#ifdef HAVE_PTHREAD_H
    	pthread_mutex_lock(&(l->mutex));
#else
        EnterCriticalSection(&(l->mutex));
#endif
#endif 
    /* TODO: keep a counter in ExList instead */
    for(lk = l->sentinel->next; lk != l->sentinel; lk = lk->next, count++);
#ifdef USE_LIST_MUTEX
#ifdef HAVE_PTHREAD_H
    	pthread_mutex_unlock(&(l->mutex));
#else
        LeaveCriticalSection(&(l->mutex));
#endif
#endif    
    return count;
}

/**
 * ExListPopFront:
 * @l:  a list
 *
 * Removes the first element in the list
 */
void
ExListPopFront(ExListPtr l)
{
    if(!ExListEmpty(l))
        ExLinkDeallocator(l, l->sentinel->next);
}

/**
 * ExListPopBack:
 * @l:  a list
 *
 * Removes the last element in the list
 */
void
ExListPopBack(ExListPtr l)
{
    if(!ExListEmpty(l))
        ExLinkDeallocator(l, l->sentinel->prev);
}

/**
 * ExListPushFront:
 * @l:  a list
 * @data:  new data
 *
 * add the new data at the beginning of the list
 *
 * Returns 1 if successful, 0 otherwise
 */
int
ExListPushFront(ExListPtr l, void *data) 
{
    ExLinkPtr lkPlace, lkNew;

    if (l == NULL)
        return(0);
    lkPlace = l->sentinel;
    /* Add the new link */
    lkNew = (ExLinkPtr) malloc(sizeof(ExLink));
    if (lkNew == NULL) {
        printf("Cannot initialize memory for new link");
        return (0);
    }
    lkNew->data = data;
    lkNew->next = lkPlace->next;
    (lkPlace->next)->prev = lkNew;
    lkPlace->next = lkNew;
    lkNew->prev = lkPlace;
    return 1;
}

/**
 * ExListPushBack:
 * @l:  a list
 * @data:  new data
 *
 * add the new data at the end of the list
 *
 * Returns 1 if successful, 0 otherwise
 */
int
ExListPushBack(ExListPtr l, void *data) 
{
    ExLinkPtr lkPlace, lkNew;

    if (l == NULL)
        return(0);
    lkPlace = l->sentinel->prev;
    /* Add the new link */
    if (NULL ==(lkNew = (ExLinkPtr )malloc(sizeof(ExLink)))) {
        printf("Cannot initialize memory for new link");
        return (0);
    }
    lkNew->data = data;
    lkNew->next = lkPlace->next;
    (lkPlace->next)->prev = lkNew;
    lkPlace->next = lkNew;
    lkNew->prev = lkPlace;
    return 1;
}

/**
 * ExLinkGetData:
 * @lk:  a link
 *
 * See Returns.
 *
 * Returns a pointer to the data referenced from this link
 */
void *
ExLinkGetData(ExLinkPtr lk)
{
    if (lk == NULL)
        return(NULL);
    return lk->data;
}

/**
 * ExListReverse:
 * @l:  a list
 *
 * Reverse the order of the elements in the list
 */
void
ExListReverse(ExListPtr l)
{
    ExLinkPtr lk;
    ExLinkPtr lkPrev;

    if (l == NULL)
        return;
    lkPrev = l->sentinel;
    for (lk = l->sentinel->next; lk != l->sentinel; lk = lk->next) {
        lkPrev->next = lkPrev->prev;
        lkPrev->prev = lk;
        lkPrev = lk;
    }
    /* Fix up the last node */
    lkPrev->next = lkPrev->prev;
    lkPrev->prev = lk;
}

/**
 * ExListSort:
 * @l:  a list
 *
 * Sort all the elements in the list
 */
void
ExListSort(ExListPtr l)
{
    ExListPtr lTemp;
    
    if (l == NULL)
        return;
    if(ExListEmpty(l))
        return;

    /* I think that the real answer is to implement quicksort, the
     * alternative is to implement some list copying procedure which
     * would be based on a list copy followed by a clear followed by
     * an insert. This is slow...
     */

    if (NULL ==(lTemp = ExListDup(l)))
        return;
    ExListClear(l);
    ExListMerge(l, lTemp);
    ExListDelete(lTemp);
    return;
}

/**
 * ExListWalk:
 * @l:  a list
 * @walker:  a processing function
 * @user:  a user parameter passed to the walker function
 *
 * Walk all the element of the first from first to last and
 * apply the walker function to it
 *
 * Returns NULL if err or list walk through, !NULL successful
 */
ExLinkPtr
ExListWalk(ExListPtr l, ExListWalker walker, const void *user) {
    ExLinkPtr lk;

    if ((l == NULL) || (walker == NULL))
        return NULL;
    for(lk = l->sentinel->next; lk != l->sentinel; lk = lk->next) {
        if((walker(lk->data, user)) == 0)
                return lk;//break;
    }
    return NULL;
}

/**
 * ExListReverseWalk:
 * @l:  a list
 * @walker:  a processing function
 * @user:  a user parameter passed to the walker function
 *
 * Walk all the element of the list in reverse order and
 * apply the walker function to it
 *
 * Returns NULL if err or list walk through, !NULL successful
 */
ExLinkPtr
ExListReverseWalk(ExListPtr l, ExListWalker walker, const void *user) {
    ExLinkPtr lk;

    if ((l == NULL) || (walker == NULL))
        return NULL;
    for(lk = l->sentinel->prev; lk != l->sentinel; lk = lk->prev) {
        if((walker(lk->data, user)) == 0)
               return lk; //break;
    }
    return NULL;
}

/**
 * ExListMerge:
 * @l1:  the original list
 * @l2:  the new list
 *
 * include all the elements of the second list in the first one and
 * clear the second list
 */
void
ExListMerge(ExListPtr l1, ExListPtr l2)
{
    ExListCopy(l1, l2);
    ExListClear(l2);
}

/**
 * ExListDup:
 * @old:  the list
 *
 * Duplicate the list
 * 
 * Returns a new copy of the list or NULL in case of error
 */
ExListPtr 
ExListDup(const ExListPtr old)
{
    ExListPtr cur;

    if (old == NULL)
        return(NULL);
    /* Hmmm, how to best deal with allocation issues when copying
     * lists. If there is a de-allocator, should responsibility lie with
     * the new list or the old list. Surely not both. I'll arbitrarily
     * set it to be the old list for the time being whilst I work out
     * the answer
     */
    if (NULL ==(cur = ExListCreate(NULL, old->linkCompare)))
        return (NULL);
    if (0 != ExListCopy(cur, old))
        return NULL;
    return cur;
}

/**
 * ExListCopy:
 * @cur:  the new list
 * @old:  the old list
 *
 * Move all the element from the old list in the new list
 * 
 * Returns 0 in case of success 1 in case of error
 */
int
ExListCopy(ExListPtr cur, const ExListPtr old)
{
    /* Walk the old tree and insert the data into the new one */
    ExLinkPtr lk;

    if ((old == NULL) || (cur == NULL))
        return(1);
    for(lk = old->sentinel->next; lk != old->sentinel; lk = lk->next) {
        if (0 !=ExListInsert(cur, lk->data)) {
            ExListDelete(cur);
            return (1);
        }
    }
    return (0);    
}
/* ExListUnique() */
/* ExListSwap */


