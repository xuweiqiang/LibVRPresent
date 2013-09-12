#ifndef __SGA_TYPES_H
#define __SGA_TYPES_H

typedef unsigned long		sga_instruction; 
typedef short				sga_coordinate;
typedef unsigned long		sga_address;
typedef unsigned char		sga_bool;

typedef signed __int64		sga_int64;
typedef unsigned __int64	sga_uint64;
typedef signed int			sga_int32;
typedef unsigned int		sga_uint32;
typedef signed short		sga_int16;
typedef unsigned short		sga_uint16;
typedef signed char			sga_int8;
typedef unsigned char		sga_uint8;

typedef struct s_sgaCommandSurface {
	HANDLE				handleID;
	sga_instruction*	currAddr;
	sga_instruction*	batchEnd;
} t_sgaCommandSurface;

typedef struct s_sgaMemoryPool {
	HANDLE			handleID;
	LPVOID			virtualAddress;
	LPVOID			physicalAddress;
} t_sgaMemoryPool;

typedef struct sgaColorFormat{
	sga_uint32		bufferFmt;
	sga_uint8		colorDepth;
} t_sgaColorFormat;

typedef struct s_sgaSurface {
	sga_uint32				m_handleID;
	t_sgaMemoryPool*		m_poolPtr;
	sga_address				addr;
	sga_address				virtualAddr;
	sga_address				guardbandAddr;
	sga_uint32				m_sizeXY;
	sga_int32				height;
	sga_int32				width;
	t_sgaColorFormat		format;
	sga_uint32				swapRB;

	int						m_refCount;
} t_sgaSurface;

typedef struct {
	int						m_x;
	int						m_y;
	int						m_w;
	int						m_h;
} t_sgaRectangle;

typedef struct {
	sga_uint32				lockID;
	HANDLE					sgaBatchHandleID;
} t_sgaDriverParameters;

typedef struct s_sgaStatistics {
	sga_uint32			Triangles;
	sga_uint32			FragmentsRaw;
	sga_uint32			FragmentsZTested;
	sga_uint32			TexelsFetched;
	sga_uint32			PixelsReads;
	sga_uint32			TextureCacheMiss;
	sga_uint32			InstrCacheMiss;
	sga_uint32			CyclesCounter;
} t_sgaStatistics;

#define IOCTL_SGA_START_BATCH_SYNC		6900
#define IOCTL_SGA_START_BATCH_ASYNC		6901
#define IOCTL_SGA_CREATE_BATCH			6902
#define IOCTL_SGA_DESTROY_BATCH			6903
#define IOCTL_SGA_WAIT_BATCH			6904
#define IOCTL_SGA_VIRT2PHYS				6905
#define IOCTL_ALLOC_MEM					6906
#define IOCTL_FREE_MEM					6907
#define IOCTL_SGA_GET_STATISTICS		6908


#endif // __SGA_TYPES_H