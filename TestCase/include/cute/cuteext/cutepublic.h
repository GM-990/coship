#ifndef CUTEPUBLIC_H
#define CUTEPUBLIC_H

#ifndef CSCUTE_DECL_EXPORT
#  if defined(__WIN32__) || defined(__WIN32) || defined(WIN32)
#    define CSCUTE_DECL_EXPORT __declspec(dllexport)
#  endif
#  ifndef CSCUTE_DECL_EXPORT
#    define CSCUTE_DECL_EXPORT
#  endif
#endif

#ifndef CSCUTE_DECL_IMPORT
#  if defined(__WIN32__) || defined(__WIN32) || defined(WIN32)
#    define CSCUTE_DECL_IMPORT __declspec(dllimport)
#  else
#    define CSCUTE_DECL_IMPORT
#  endif
#endif

#define CSCUTE_BEGIN_C_HEADER  extern "C" {
#define CSCUTE_END_C_HEADER    }
                
#define CSCUTE_BEGIN_CPP_HEADER   extern "C++" {
#define CSCUTE_END_CPP_HEADER     }

#define CSCUTE_C_EXPORT extern "C" CSCUTE_DECL_EXPORT
#define CSCUTE_C_IMPORT extern "C" CSCUTE_DECL_IMPORT

#ifdef __cplusplus

#define CSCUTE_PRIVATE_CLASS(Class) \
                class Class##Private;

#define CSCUTE_DECLARE_PRIVATE(Class) \
                friend class Class##Private; \
                private: \
                Class##Private* m_ptr;

#endif

#endif // CUTEPUBLIC_H
