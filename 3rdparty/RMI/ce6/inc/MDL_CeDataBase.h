//
// Copyright (c) 2005  Advanced Micro Device, AMD.  All rights reserved.
// Copyright (c) 2005  BSQUARE Corporation.  All rights reserved.
// 
// Module Name:
//
//     mdl_CeDataBase.h
//
// Abstract:
//
//     Defines the API to the Media Data Library (MDL)
//

#ifndef MTP_CEDB__H
#define MTP_CEDB__H
#include "MDL_CeDbDefs.h"


typedef mdl_ResultCodeType (*mdl_EventCallback)( mdl_EventStructType *);
#define NUMCLIENTS 5

typedef struct mdlUserDataRecord_s 
{
  MDL_UINT32             m_bSet;      /* is this record set? */
  mdl_ClientId           m_ClientId; /* unique identifier to user data */
  mdl_EventCallback      m_pfEventCallback;
  MDL_UINT32             m_Param1;
  MDL_UINT32             m_bLocked; /* Does this record have the library locked? */
  mdl_GroupDataType     *groupData;
  HANDLE                 hDatabase;

} mdlUserDataRecord_t;

// Set the database filename
void mdl_setDBFileName(TCHAR *dbfile);

// This function must be called before the object is used.
void mdl_InitObject(mdl_ObjectPropertiesType * Objectprops);

// Add an object to the database
mdl_ResultCodeType mdl_AddObject(mdl_ClientId Client, 
                                 mdl_PUOI *pObjd, 
                                 mdl_ObjectPropertiesType * ObjectProp);

// Create a blank object in the database
mdl_ResultCodeType mdl_CreateObject(mdl_ClientId Client, 
                                    mdl_PUOI *pObjd);

// Remove an object from the database
mdl_ResultCodeType mdl_RemoveObject(mdl_ClientId Client, 
                                    mdl_PUOI Object);

// Set all properties for an object
mdl_ResultCodeType mdl_SetAllPropertiesForObject(mdl_ClientId Client,
                                                 mdl_PUOI Object, 
                                                 mdl_ObjectPropertiesType *ObjectProps);

// Get all properties for an object
mdl_ResultCodeType mdl_GetAllPropertiesForObject(mdl_ClientId Client, 
                                                 mdl_PUOI Object, 
                                                 mdl_ObjectPropertiesType *ObjectProps);

// Return a list of all objects in the library, ObjectList will point to 
// an array allocated using LocalAlloc, client must free with LocalFree.
mdl_ResultCodeType mdl_ListAllObjects(mdl_ClientId Client,
                                      mdl_PUOI **ObjectList,
                                      MDL_UINT32 *NumberOfObjects);

// Signal an event to a client
mdl_ResultCodeType SignalEvent(mdl_ClientId Client, mdl_EventStructType *Event);

// Set a single property of an object.
mdl_ResultCodeType mdl_SetObjectProperty(mdl_ClientId Client, 
                                         mdl_PUOI Object, 
                                         mdl_ObjectPropertyNamesType Property, 
                                         void *pProperty);

  // Set a single property of an object, which is returned in pProperty.
mdl_ResultCodeType mdl_GetObjectProperty(mdl_ClientId Client, 
                                         mdl_PUOI Object,
                                         mdl_ObjectPropertyNamesType Property, 
                                         void *pProperty);


/**
 * Requests access to the Media Database Library by a client. Used after a client has been registered.
 * If a successful call to mdl_RegisterLibraryClient() has not been made before this function is called
 * then the function will return #MDL_FAILURE.
 * mdl_RequestLibraryAccess() should be used in conjuction with mdl_EndLibraryAccess() to delimit
 * atomic accesses to the library. Between calls to mdl_RequestLibraryAccess() and mdl_EndLibraryAccess() 
 * the client has exclusive access to the library. 
 * 
 * \param Client the client that is requesting access to the library.
 * \param RequestType if RequestType is #MDL_BLOCKING then mdl_RequestLibraryAccess() will not return until 
 *                     access to the library by the requested client is possible or an error is detected.
 *                     if RequestType is #MDL_NON_BLOCKING then mdl_RequestLibraryAccess() will return immediately
 *                     signalling whether access is possible or not in the return value.
 * \returns 
 *              \li #MDL_SUCCESS if RequestType is #MDL_BLOCKING or #MDL_NON_BLOCKING and access is available
 *              \li #MDL_BLOCKED if RequestType is #MDL_NON_BLOCKING and access is not available because
 *          another client is currently accessing the library
 *              \li #MDL_CLIENT_NOT_REGISTERED if the client has not previously registered with a call to mdl_RegisterLibraryClient()
 *              \li #MDL_FAILURE otherwise
 *                                                              th
 */ 
mdl_ResultCodeType mdl_RequestLibraryAccess(mdl_ClientId Client,
                                            mdl_RequestType RequestType);

/**
 * Signals that a client has finished accessing the library for the time being. Once mdl_EndLibraryAccess()
 * has been called, any changes that were made by the client are signalled to any other registered library 
 * clients.
 * 
 * \param Client the client that has finished accesing the library.
 * 
 * \returns 
 *                  \li #MDL_SUCCESS if successful,
 *                  \li #MDL_BLOCKED if the client has not previously successfully called mdl_RequestLibraryAccess(),
 *          \li #MDL_CLIENT_NOT_REGISTERED if the client has not previously registered with a call to mdl_RegisterLibraryClient()
 *                  \li #MDL_FAILURE otherwise
 */
mdl_ResultCodeType mdl_EndLibraryAccess(mdl_ClientId Client);

/**
 * Used to register a client with the library.
 * and that are allowed to request access to the library.
 * 
 *              \li #MDL_SUCCESS if successful 
 *              \li #MDL_FAILURE otherwise
 */ 
mdl_ResultCodeType mdl_RegisterLibraryClient(mdl_ClientId *ClientId,
                                             char *ClientDescription,
                                             mdl_ResultCodeType (*mdl_EventCallback)( mdl_EventStructType *));

/**
 * Used to remove a client from the list of clients that will be notified of library changes
 * and that are allowed to request access to the library.
 * 
 * \param ClientToDeregister The client ID that was supplied by the original call to mdl_RegisterLibraryClient()
 * and that now no longer requires access to the library or notifications of changes.
 * 
 * \returns 
 *              \li #MDL_SUCCESS if successful 
 *              \li #MDL_CLIENT_NOT_REGISTERED if the client has not previously registered with a call to mdl_RegisterLibraryClient()
 *              \li #MDL_FAILURE otherwise
 */ 
mdl_ResultCodeType mdl_DeregisterLibraryClient(mdl_ClientId ClientToDeregister);

 /* Requests access to the Media Database Library by a client. Used after a client has been registered.
/ * If a successful call to mdl_RegisterLibraryClient() has not been made before this function is called
 * then the function will return #MDL_FAILURE.
 * mdl_RequestLibraryAccess() should be used in conjuction with mdl_EndLibraryAccess() to delimit
 * atomic accesses to the library. Between calls to mdl_RequestLibraryAccess() and mdl_EndLibraryAccess() 
 * the client has exclusive access to the library. 
 * 
 * \param Client the client that is requesting access to the library.
 * \param RequestType if RequestType is #MDL_BLOCKING then mdl_RequestLibraryAccess() will not return until 
 *                     access to the library by the requested client is possible or an error is detected.
 *                     if RequestType is #MDL_NON_BLOCKING then mdl_RequestLibraryAccess() will return immediately
 *                     signalling whether access is possible or not in the return value.
 * \returns 
 *              \li #MDL_SUCCESS if RequestType is #MDL_BLOCKING or #MDL_NON_BLOCKING and access is available
 *              \li #MDL_BLOCKED if RequestType is #MDL_NON_BLOCKING and access is not available because
 *          another client is currently accessing the library
 *              \li #MDL_CLIENT_NOT_REGISTERED if the client has not previously registered with a call to mdl_RegisterLibraryClient()
 *              \li #MDL_FAILURE otherwise
 *                                                              th
 */ 
mdl_ResultCodeType mdl_RequestLibraryAccess(mdl_ClientId Client,
                                            mdl_RequestType RequestType);

/*
Methods not needed

// This function frees all the properties pointed to by ObjectProps;
void mdl_FreeObject(mdl_ObjectPropertiesType *ObjectProps);


*/
#endif