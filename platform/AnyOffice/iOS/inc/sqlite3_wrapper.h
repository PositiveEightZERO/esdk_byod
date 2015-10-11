#ifndef _SQLITE_WRAPPER_H
#define  _SQLITE_WRAPPER_H

//#include "anyoffice_sqlite3.h"
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif

SQLITE_API int sqlite3_open_v2_s(
          const char *filename,   /* Database filename (UTF-8) */
          sqlite3 **ppDb,         /* OUT: SQLite db handle */
          int flags,              /* Flags */
          const char *zVfs        /* Name of VFS module to use */
          );

#endif
