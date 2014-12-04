/******************************************************************************
 Copyright (C), 2013-2063, Haihua Electronics Enterprise (China) Co., Ltd.
******************************************************************************
File Name     : he_type.h
Version       : Initial Draft
Author        : No.201328
Created       : 2013/6/21
Last Modified :
Description   : 系统共用的数据类型定义
Function List :
History       :
******************************************************************************/
#ifndef __HE_TYPE_H__
#define __HE_TYPE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*----------------------------------------------*
 * 基本数据类型定义，应用层和内核代码均使用     *
 *----------------------------------------------*/
/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_1_1 */
/** @{ */  /** <!-- 【数据类型】 */

typedef unsigned char           HE_U8;
typedef unsigned char           HE_UCHAR;
typedef unsigned short          HE_U16;
typedef unsigned int            HE_U32;

typedef signed char             HE_S8;
typedef short                   HE_S16;
typedef int                     HE_S32;

#ifndef _M_IX86
typedef unsigned long long      HE_U64;
typedef long long               HE_S64;
#else
typedef __int64                 HE_U64;
typedef __int64                 HE_S64;
#endif

typedef char                    HE_CHAR;
typedef char*                   HE_PCHAR;

typedef float                   HE_FLOAT;
typedef double                  HE_DOUBLE;
/*typedef void                    HE_VOID;*/
#define HE_VOID 		void

typedef unsigned long           HE_SIZE_T;
typedef unsigned long           HE_LENGTH_T;

typedef HE_U32                  HE_HANDLE;


/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
typedef enum {
    HE_FALSE    = 0,
    HE_TRUE     = 1,
} HE_BOOL;

#ifndef NULL
#define NULL             0L
#endif
#define HE_NULL          0L
#define HE_NULL_PTR      0L

#define HE_SUCCESS       0
#define HE_FAILURE       (-1)

#define HE_INVALID_HANDLE (0xffffffff)

#define HE_OS_LINUX      0xabcd
#define HE_OS_WIN32      0xcdef

#ifdef _WIN32
#define HE_OS_TYPE      HE_OS_WIN32
#else
#define __OS_LINUX__
#define HE_OS_TYPE      HE_OS_LINUX
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HE_TYPE_H__ */


