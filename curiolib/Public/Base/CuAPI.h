
#ifndef CURIO_API_H
#define CURIO_API_H

#ifdef CURIO_STATICLIB
#  define CURIO_API
#  define CURIO_NO_EXPORT
#else
#  ifndef CURIO_API
#    ifdef curiolib_EXPORTS
        /* We are building this library */
#      define CURIO_API 
#    else
        /* We are using this library */
#      define CURIO_API 
#    endif
#  endif

#  ifndef CURIO_NO_EXPORT
#    define CURIO_NO_EXPORT 
#  endif
#endif

#ifndef CURIO_DEPRECATED
#  define CURIO_DEPRECATED __declspec(deprecated)
#endif

#ifndef CURIO_DEPRECATED_EXPORT
#  define CURIO_DEPRECATED_EXPORT CURIO_API CURIO_DEPRECATED
#endif

#ifndef CURIO_DEPRECATED_NO_EXPORT
#  define CURIO_DEPRECATED_NO_EXPORT CURIO_NO_EXPORT CURIO_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef CURIO_NO_DEPRECATED
#    define CURIO_NO_DEPRECATED
#  endif
#endif

#endif /* CURIO_API_H */
