#ifndef _LOG_H_
#define _LOG_H_

#ifdef LOG_PRINT
  #include <stdio.h>
  #include <stdint.h>
  #include <sys/types.h>
  #include <unistd.h>
  #define __STDC_FORMAT_MACROS
  #include <inttypes.h>
  #ifdef LOG_PRINT_TIMESTAMP
    #include <time.h>
    #ifdef LOG_TAG
      #define TAG(tag) #tag
      #ifdef LOG_LEVEL_DEBUG
        #define DEBUG(...) \
        do { \
            struct timespec ts; \
            clock_gettime(CLOCK_REALTIME, &ts); \
            struct tm lt; \
            localtime_r(&ts.tv_sec, &lt); \
            fprintf(stdout, "[%04d/%02d/%02d %02d:%02d:%02d.%03ld] ", lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec, ts.tv_nsec/1000000); \
            fprintf(stdout, "(%d:" LOG_TAG ") ", getpid()); \
            fprintf(stdout, __VA_ARGS__); \
            fflush(stdout); \
        } while(0)
      #else /* LOG_LEVEL_DEBUG */
        #define DEBUG(...)
      #endif
      #define LOG(...) \
      do { \
          struct timespec ts; \
          clock_gettime(CLOCK_REALTIME, &ts); \
          struct tm lt; \
          localtime_r(&ts.tv_sec, &lt); \
          fprintf(stdout, "[%04d/%02d/%02d %02d:%02d:%02d.%03ld] ", lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec, ts.tv_nsec/1000000); \
          fprintf(stdout, "(%d:" LOG_TAG ") ", getpid()); \
          fprintf(stdout, __VA_ARGS__); \
          fflush(stdout); \
      } while(0)
      #define ERR(...) \
      do { \
          struct timespec ts; \
          clock_gettime(CLOCK_REALTIME, &ts); \
          struct tm lt; \
          localtime_r(&ts.tv_sec, &lt); \
          fprintf(stderr, "[%04d/%02d/%02d %02d:%02d:%02d.%03ld] ", lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec, ts.tv_nsec/1000000); \
          fprintf(stderr, "(%d:" LOG_TAG ") ", getpid()); \
          fprintf(stderr, __VA_ARGS__); \
          fflush(stderr); \
      } while(0)
    #else  /* LOG_TAG */
      #ifdef LOG_LEVEL_DEBUG
      #define DEBUG(...) \
        do { \
            struct timespec ts; \
            clock_gettime(CLOCK_REALTIME, &ts); \
            struct tm lt; \
            localtime_r(&ts.tv_sec, &lt); \
            fprintf(stdout, "[%04d/%02d/%02d %02d:%02d:%02d.%03ld] ", lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec, ts.tv_nsec/1000000); \
            fprintf(stdout, "(%d) ", getpid()); \
            fprintf(stdout, __VA_ARGS__); \
            fflush(stdout); \
        } while(0)
      #else /* LOG_LEVEL_DEBUG */
        #define DEBUG(...)
      #endif
      #define LOG(...) \
      do { \
          struct timespec ts; \
          clock_gettime(CLOCK_REALTIME, &ts); \
          struct tm lt; \
          localtime_r(&ts.tv_sec, &lt); \
          fprintf(stdout, "[%04d/%02d/%02d %02d:%02d:%02d.%03ld] ", lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec, ts.tv_nsec/1000000); \
          fprintf(stdout, "(%d) ", getpid()); \
          fprintf(stdout, __VA_ARGS__); \
          fflush(stdout); \
      } while(0)
      #define ERR(...) \
      do { \
          struct timespec ts; \
          clock_gettime(CLOCK_REALTIME, &ts); \
          struct tm lt; \
          localtime_r(&ts.tv_sec, &lt); \
          fprintf(stderr, "[%04d/%02d/%02d %02d:%02d:%02d.%03ld] ", lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec, ts.tv_nsec/1000000); \
          fprintf(stderr, "(%d) ", getpid()); \
          fprintf(stderr, __VA_ARGS__); \
          fflush(stderr); \
      } while(0)
    #endif /* LOG_TAG */
  #else /* LOG_PRINT_TIMESTAMP */
    #ifdef LOG_TAG
      #define TAG(tag) #tag
      #ifdef LOG_LEVEL_DEBUG
        #define DEBUG(...) (fprintf(stdout, "(" LOG_TAG ") " __VA_ARGS__))
      #else /* LOG_LEVEL_DEBUG */
        #define DEBUG(...)
      #endif
      #define LOG(...) (fprintf(stdout, "(" LOG_TAG ") " __VA_ARGS__))
      #define ERR(...) (fprintf(stderr, "(" LOG_TAG ") " __VA_ARGS__))
    #else  /* LOG_TAG */
      #ifdef LOG_LEVEL_DEBUG
        #define DEBUG(...) (fprintf(stdout, __VA_ARGS__))
      #else /* LOG_LEVEL_DEBUG */
        #define DEBUG(...)
      #endif
      #define LOG(...) (fprintf(stdout, __VA_ARGS__))
      #define ERR(...) (fprintf(stderr, __VA_ARGS__))
    #endif  /* LOG_TAG */
  #endif /* LOG_PRINT_TIMESTAMP */
#else
  #define DEBUG(...)
  #define LOG(...)
  #define ERR(...)
#endif /* LOG_PRINT */

#endif /* _LOG_H_ */
