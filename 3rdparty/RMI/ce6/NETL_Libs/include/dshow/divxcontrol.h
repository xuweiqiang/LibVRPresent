#ifndef __DIVX_CONTROL_H__
#define __DIVX_CONTROL_H__


/*
** @func unsigned int | pfCompErrorCB_t | Application callback that a component reports errors to.
** @rvalue S_OK | App successfully processed error.
** @parm HRESULT | eStatus | Major error status code; 
** @parm unsigned int | uiSubErrorCode | Sub error code.
*/
typedef unsigned int (*pfCompErrorCB_t)(HRESULT eStatus, unsigned int uiSubErrorCode);

// {15DE6C3A-4C43-45cb-9E29-A602B3E6643B}
DEFINE_GUID(IID_IDivXControl, 
0x15de6c3a, 0x4c43, 0x45cb, 0x9e, 0x29, 0xa6, 0x2, 0xb3, 0xe6, 0x64, 0x3b);

#define DIVX_STATUS_ERROR       0x00001000 /* @demem DivX error */
#define DIVX_STATUS_UNSUPPORTED 0x00001001 /* @demem DivX is unsupported */
#define DIVX_STATUS_EXPIRED     0x00001002 /* @demem DivX reports expired */
#define DIVX_STATUS_INITERROR   0x00001003 /* @demem DivX initialization error */
#define DIVX_STATUS_NOAUTH      0x00001004 /* @demem DivX missing authentication */
#define DIVX_STATUS_RENTAL      0x00001005 /* @demem DivX rental confirmation required */

#ifndef __IDivXControl_INTERFACE_DEFINED__
#define __IDivXControl_INTERFACE_DEFINED__

/* interface IDivXControl */
/* [uuid][object] */ 


EXTERN_C const IID IID_IDivXControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("15DE6C3A-4C43-45cb-9E29-A602B3E6643B")
    IDivXControl : public IUnknown
    {
    public:
	      /*!
		      Sets the Error Callback routine,

		      @param pfCB (in) function pointer to an error callback routine.
		      @return S_OK if successful or error code
	      */
        virtual HRESULT STDMETHODCALLTYPE SetErrorCallBack( 
            pfCompErrorCB_t pfCB) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRegistrationCode( 
            wchar_t *pwcRegCode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRentalLimits( 
            unsigned char *RentalCount, unsigned char *RentalLimit) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDivXControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDivXControl * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDivXControl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDivXControl * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetErrorCallBack )( 
            IDivXControl * This,
            pfCompErrorCB_t pfCB);
        
        HRESULT ( STDMETHODCALLTYPE *GetRegistrationCode )( 
            IDivXControl * This,
            wchar_t *pwcRegCode);
        
        HRESULT ( STDMETHODCALLTYPE *GetRentalLimits )( 
            IDivXControl * This,
            unsigned char *RentalCount, unsigned char *RentalLimit);
        
        END_INTERFACE
    } IDivXControlVtbl;

    interface IDivXControl
    {
        CONST_VTBL struct IDivXControlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDivXControl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDivXControl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDivXControl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDivXControl_SetErrorCallBack(This,pfCB)	\
    (This)->lpVtbl -> SetErrorCallBack(This,pfCB)

#define IDivXControl_GetRegistrationCode(This,pwcRegCode)	\
    (This)->lpVtbl -> GetRegistrationCode(This,pwcRegCode)

#define IDivXControl_GetRentalLimits(This,RentalCount,RentalLimit)	\
    (This)->lpVtbl -> GetRentalLimits(This,RentalCount,RentalLimit)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDivXControl_SetErrorCallBack_Proxy( 
    IDivXControl * This,
    pfCompErrorCB_t pfCB);


void __RPC_STUB IDivXControl_SetErrorCallBack_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDivXControl_GetRegistrationCode_Proxy( 
    IDivXControl * This,
    wchar_t *pwcRegCode);


void __RPC_STUB IDivXControl_GetRegistrationCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDivXControl_GetRentalLimits_Proxy( 
    IDivXControl * This,
    unsigned char *RentalCount, unsigned char *RentalLimit);


void __RPC_STUB IDivXControl_GetRentalLimits_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDivXControl_INTERFACE_DEFINED__ */

#endif //__DIVX_CONTROL_H__
