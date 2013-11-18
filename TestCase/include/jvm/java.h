/*
 * @(#)java.h	1.3 01/11/13
 *
 * Copyright ?1994-2002 Sun Microsystems, Inc.  All rights reserved.  
 *
 * PROPRIETARY/CONFIDENTIAL
 *
 * Use is subject to license terms.
 *
 */

/*
 * Utility functions for ANSI platforms
 */
#include <jni.h>
#include <pthread.h>

#ifndef _ANSI_JAVA_H
#define _ANSI_JAVA_H

/* Java startup */
int ansiJavaMain(int argc, const char **argv);

/*get pthread id by java thread object*/
pthread_t CSJvmGetJavaThreadId (JNIEnv *env, jobject thread);

#endif /* _ANSI_JAVA_H */
