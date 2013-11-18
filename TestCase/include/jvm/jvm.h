/*
 * @(#)jvm.h	1.6 01/11/13
 *
 * Copyright © 1994-2002 Sun Microsystems, Inc.  All rights reserved.  
 *
 * PROPRIETARY/CONFIDENTIAL
 *
 * Use is subject to license terms.
 *
 */

#ifndef _EXPORT_JVM_H_
#define _EXPORT_JVM_H_

#include "javavm/include/porting/defs.h"
#include "javavm/include/porting/ansi/stdarg.h"
#include "javavm/include/porting/ansi/stddef.h"
#include "javavm/include/porting/io.h"
#include "javavm/include/porting/net.h"
#include "javavm/include/utils.h"
#include "javavm/include/string.h"	/* CVMstringIntern() */

#include "javavm/export/jni.h"

/* Avoid conflicts with our own types */

#include "javavm/include/jvm2romnatives.h"
#include "javavm/jdk-export/jvm.h"
#include "javavm/include/jvm2cvm.h"

/* This was added for JVMDI and JVMPI support of starting "system
   threads" -- threads for which the main function is a raw C
   function. nativeFunc must not be NULL, If the nativeFuncArg
   argument is non-NULL, it must be allocated on the heap by the
   caller and deallocated by the user's code (likely in the user's
   spawned thread). */
void
JVM_StartSystemThread(JNIEnv *env, jobject thread,
		      void(*nativeFunc)(void *), void* nativeFuncArg);

#endif /* !_EXPORT_JVM_H_ */
