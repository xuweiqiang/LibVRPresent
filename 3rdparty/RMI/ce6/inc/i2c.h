#ifndef __IIC__H_
#define __IIC__H_

extern HANDLE IICOpen(void);
extern void  IICClose(HANDLE handle);
extern BOOL  IICWriteData(HANDLE handle, BYTE address, BYTE reg, BYTE *inBuffPtr, ULONG wtLen);
extern BOOL  IICReadData(HANDLE handle, BYTE address, BYTE reg, BYTE *outBuffPtr, ULONG rdLen);

#endif
