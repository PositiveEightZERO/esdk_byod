/******************************************************************************

  Copyright (C), 2001-2011, Huawei Tech. Co., Ltd.

 ******************************************************************************
  File Name     : anyoffice_sqlite3.h
  Version       : Initial Draft
  Author        : zhangtailei 00218689
  Created       : 2015/3/25
  Last Modified :
  Description   : SDK对外sqlite封装接口头文件
  Function List :
  History       :
  1.Date        : 2015/3/25
    Author      : zhangtailei 00218689
    Modification: Created file

******************************************************************************/

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/
#ifndef __ANYOFFICE_SQLITE3_IMPL__
#define __ANYOFFICE_SQLITE3_IMPL__

#include <stdarg.h>     /* Needed for the definition of va_list */

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif

#endif /* __cplusplus */


#include <stdio.h>
/*****************************************************************
                    sqlite3结构桩定义
*****************************************************************/

#ifndef sqlite3
#define sqlite3 void
#endif


typedef void (*AnyOffice_sqlite3_destructor_type)(void*);
#define sqlite3_destructor_type AnyOffice_sqlite3_destructor_type

#define SQLITE_STATIC      ((sqlite3_destructor_type)0)
#define SQLITE_TRANSIENT   ((sqlite3_destructor_type)-1)

typedef int (*AnyOffice_sqlite3_callback)(void*,int,char**, char**);
#define sqlite3_callback AnyOffice_sqlite3_callback

typedef long long int AnyOffice_sqlite_int64;
typedef unsigned long long int AnyOffice_sqlite_uint64;
#define sqlite_int64 AnyOffice_sqlite_int64
#define sqlite3_int64 sqlite_int64
#define sqlite_uint64 AnyOffice_sqlite_uint64
#define sqlite3_uint64 sqlite_uint64

#ifndef SQLITE_API
#define SQLITE_API
#endif

#ifndef sqlite3_stmt
#define sqlite3_stmt void
#endif

#ifndef sqlite3_value
#define sqlite3_value void
#endif

#ifndef sqlite3_vfs
#define sqlite3_vfs void
#endif

#ifndef sqlite3_mutex
#define sqlite3_mutex void
#endif

#ifndef sqlite3_backup
#define sqlite3_backup void
#endif

#ifndef sqlite3_context
#define sqlite3_context void
#endif

#ifndef sqlite3_module
#define sqlite3_module void
#endif

#ifndef sqlite3_blob
#define sqlite3_blob void
#endif


// TODO:
//char *AnyOffice_sqlite3_temp_directory = sqlite3_temp_directory;
//char *AnyOffice_sqlite3_data_directory = sqlite3_data_directory;


const char * AnyOffice_sqlite3_libversion(void);

const char *AnyOffice_sqlite3_sourceid(void);

int AnyOffice_sqlite3_libversion_number(void);

int AnyOffice_sqlite3_compileoption_used(const char *zOptName);

const char *AnyOffice_sqlite3_compileoption_get(int N);

int AnyOffice_sqlite3_threadsafe(void);

int AnyOffice_sqlite3_close(sqlite3 *db);

int AnyOffice_sqlite3_close_v2(sqlite3 *db);

int AnyOffice_sqlite3_exec(
  sqlite3 *db,                /* The database on which the SQL executes */
  const char *zSql,           /* The SQL to be executed */
  sqlite3_callback xCallback, /* Invoke this callback routine */
  void *pArg,                 /* First argument to xCallback() */
  char **pzErrMsg             /* Write error messages here */
);

int AnyOffice_sqlite3_initialize(void);


int AnyOffice_sqlite3_shutdown(void);

int AnyOffice_sqlite3_os_init(void);

int AnyOffice_sqlite3_os_end(void);

int AnyOffice_sqlite3_config(int op, ...);

int AnyOffice_sqlite3_extended_result_codes(sqlite3 *db, int onoff);

sqlite_int64 AnyOffice_sqlite3_last_insert_rowid(sqlite3 *db);

int AnyOffice_sqlite3_changes(sqlite3 *db);

int AnyOffice_sqlite3_total_changes(sqlite3 *db);

void AnyOffice_sqlite3_interrupt(sqlite3 *db);

int AnyOffice_sqlite3_complete(const char *zSql);

#ifndef SQLITE_OMIT_UTF16
int AnyOffice_sqlite3_complete16(const void *zSql);
#endif

int AnyOffice_sqlite3_busy_handler(
  sqlite3 *db,
  int (*xBusy)(void*,int),
  void *pArg
);

int AnyOffice_sqlite3_busy_timeout(sqlite3 *db, int ms);

int AnyOffice_sqlite3_get_table(
  sqlite3 *db,                /* The database on which the SQL executes */
  const char *zSql,           /* The SQL to be executed */
  char ***pazResult,          /* Write the result table here */
  int *pnRow,                 /* Write the number of rows in the result here */
  int *pnColumn,              /* Write the number of columns of result here */
  char **pzErrMsg             /* Write error messages here */
);

void AnyOffice_sqlite3_free_table(
  char **azResult            /* Result returned from from sqlite3_get_table() */
);

char *AnyOffice_sqlite3_mprintf(const char *zFormat, ...);

char *AnyOffice_sqlite3_vmprintf(const char *zFormat, va_list ap);

char *AnyOffice_sqlite3_vsnprintf(int n, char *zBuf, const char *zFormat, va_list ap);

char *AnyOffice_sqlite3_snprintf(int n, char *zBuf, const char *zFormat, ...);

void *AnyOffice_sqlite3_malloc(int n);

void *AnyOffice_sqlite3_realloc(void *pOld, int n);

void AnyOffice_sqlite3_free(void *p);

sqlite3_int64 AnyOffice_sqlite3_memory_used(void);

sqlite3_int64 AnyOffice_sqlite3_memory_highwater(int resetFlag);

void AnyOffice_sqlite3_randomness(int N, void *pBuf);

int AnyOffice_sqlite3_set_authorizer(
  sqlite3 *db,
  int (*xAuth)(void*,int,const char*,const char*,const char*,const char*),
  void *pArg
);

void *AnyOffice_sqlite3_trace(sqlite3 *db, void (*xTrace)(void*,const char*), void *pArg);

void *AnyOffice_sqlite3_profile(
  sqlite3 *db,
  void (*xProfile)(void*,const char*,sqlite_uint64),
  void *pArg
);

void AnyOffice_sqlite3_progress_handler(
  sqlite3 *db, 
  int nOps,
  int (*xProgress)(void*), 
  void *pArg
);

#ifndef ANYOFFICE_ANDROID
int AnyOffice_sqlite3_open_s(
          const char *filename,   /* Database filename (UTF-8) */
          sqlite3 **ppDb,         /* OUT: SQLite db handle */
          int flags,              /* Flags */
          const char *zVfs,        /* Name of VFS module to use */
          const char* username
          );
#else
int AnyOffice_sqlite3_open_s(
          const char *filename,   /* Database filename (UTF-8) */
          sqlite3 **ppDb,         /* OUT: SQLite db handle */
          int flags,              /* Flags */
          const char *zVfs,        /* Name of VFS module to use */
          const char* username,    /* User Name */
          const char* deviceID      /* DeviceID */
          );
#endif

int AnyOffice_sqlite3_open(
  const char *zFilename, 
  sqlite3 **ppDb 
);

#ifndef SQLITE_OMIT_UTF16
int AnyOffice_sqlite3_open16(
  const void *zFilename, 
  sqlite3 **ppDb
);
#endif

int AnyOffice_sqlite3_open_v2(
  const char *filename,   /* Database filename (UTF-8) */
  sqlite3 **ppDb,         /* OUT: SQLite db handle */
  int flags,              /* Flags */
  const char *zVfs        /* Name of VFS module to use */
);

const char *AnyOffice_sqlite3_uri_parameter(const char *zFilename, const char *zParam);

int AnyOffice_sqlite3_uri_boolean(const char *zFilename, const char *zParam, int bDflt);

sqlite3_int64 AnyOffice_sqlite3_uri_int64(
  const char *zFilename,    /* Filename as passed to xOpen */
  const char *zParam,       /* URI parameter sought */
  sqlite3_int64 bDflt       /* return if parameter is missing */
);

int AnyOffice_sqlite3_errcode(sqlite3 *db);

int AnyOffice_sqlite3_extended_errcode(sqlite3 *db);

const char *AnyOffice_sqlite3_errmsg(sqlite3 *db);

#ifndef SQLITE_OMIT_UTF16
const void *AnyOffice_sqlite3_errmsg16(sqlite3 *db);
#endif

const char *AnyOffice_sqlite3_errstr(int rc);

int AnyOffice_sqlite3_limit(sqlite3 *db, int limitId, int newLimit);

int AnyOffice_sqlite3_prepare(
  sqlite3 *db,              /* Database handle. */
  const char *zSql,         /* UTF-8 encoded SQL statement. */
  int nBytes,               /* Length of zSql in bytes. */
  sqlite3_stmt **ppStmt,    /* OUT: A pointer to the prepared statement */
  const char **pzTail       /* OUT: End of parsed string */
);

int AnyOffice_sqlite3_prepare_v2(
  sqlite3 *db,              /* Database handle. */
  const char *zSql,         /* UTF-8 encoded SQL statement. */
  int nBytes,               /* Length of zSql in bytes. */
  sqlite3_stmt **ppStmt,    /* OUT: A pointer to the prepared statement */
  const char **pzTail       /* OUT: End of parsed string */
);

#ifndef SQLITE_OMIT_UTF16
int AnyOffice_sqlite3_prepare16(
  sqlite3 *db,              /* Database handle. */ 
  const void *zSql,         /* UTF-16 encoded SQL statement. */
  int nBytes,               /* Length of zSql in bytes. */
  sqlite3_stmt **ppStmt,    /* OUT: A pointer to the prepared statement */
  const void **pzTail       /* OUT: End of parsed string */
);
#endif

int AnyOffice_sqlite3_prepare16_v2(
  sqlite3 *db,              /* Database handle. */ 
  const void *zSql,         /* UTF-16 encoded SQL statement. */
  int nBytes,               /* Length of zSql in bytes. */
  sqlite3_stmt **ppStmt,    /* OUT: A pointer to the prepared statement */
  const void **pzTail       /* OUT: End of parsed string */
);

const char *AnyOffice_sqlite3_sql(sqlite3_stmt *pStmt);

int AnyOffice_sqlite3_stmt_readonly(sqlite3_stmt *pStmt);

int AnyOffice_sqlite3_stmt_busy(sqlite3_stmt *pStmt);

int AnyOffice_sqlite3_bind_blob(
  sqlite3_stmt *pStmt, 
  int i, 
  const void *zData, 
  int nData, 
  void (*xDel)(void*)
);

int AnyOffice_sqlite3_bind_double(sqlite3_stmt *pStmt, int i, double rValue);

int AnyOffice_sqlite3_bind_int(sqlite3_stmt *p, int i, int iValue);

int AnyOffice_sqlite3_bind_int64(sqlite3_stmt *pStmt, int i, sqlite_int64 iValue);

int AnyOffice_sqlite3_bind_null(sqlite3_stmt *pStmt, int i);

int AnyOffice_sqlite3_bind_text( 
  sqlite3_stmt *pStmt, 
  int i, 
  const char *zData, 
  int nData, 
  void (*xDel)(void*)
);

#ifndef SQLITE_OMIT_UTF16
int AnyOffice_sqlite3_bind_text16(
  sqlite3_stmt *pStmt, 
  int i, 
  const void *zData, 
  int nData, 
  void (*xDel)(void*)
);
#endif

int AnyOffice_sqlite3_bind_value(sqlite3_stmt *pStmt, int i, const sqlite3_value *pValue);

int AnyOffice_sqlite3_bind_zeroblob(sqlite3_stmt *pStmt, int i, int n);

int AnyOffice_sqlite3_bind_parameter_count(sqlite3_stmt *pStmt);

const char *AnyOffice_sqlite3_bind_parameter_name(sqlite3_stmt *pStmt, int i);

int AnyOffice_sqlite3_bind_parameter_index(sqlite3_stmt *pStmt, const char *zName);

int AnyOffice_sqlite3_clear_bindings(sqlite3_stmt *pStmt);

int AnyOffice_sqlite3_column_count(sqlite3_stmt *pStmt);

const char *AnyOffice_sqlite3_column_name(sqlite3_stmt *pStmt, int N);

#ifndef SQLITE_OMIT_UTF16
const void *AnyOffice_sqlite3_column_name16(sqlite3_stmt *pStmt, int N);
#endif

#ifndef SQLITE_OMIT_DECLTYPE
const char *AnyOffice_sqlite3_column_decltype(sqlite3_stmt *pStmt, int N);

#ifndef SQLITE_OMIT_UTF16
const void *AnyOffice_sqlite3_column_decltype16(sqlite3_stmt *pStmt, int N);
#endif
#endif

int AnyOffice_sqlite3_step(sqlite3_stmt *pStmt);

int AnyOffice_sqlite3_data_count(sqlite3_stmt *pStmt);

const void *AnyOffice_sqlite3_column_blob(sqlite3_stmt *pStmt, int i);

int AnyOffice_sqlite3_column_bytes(sqlite3_stmt *pStmt, int i);

int AnyOffice_sqlite3_column_bytes16(sqlite3_stmt *pStmt, int i);

double AnyOffice_sqlite3_column_double(sqlite3_stmt *pStmt, int i);

int AnyOffice_sqlite3_column_int(sqlite3_stmt *pStmt, int i);

sqlite_int64 AnyOffice_sqlite3_column_int64(sqlite3_stmt *pStmt, int i);

const unsigned char *AnyOffice_sqlite3_column_text(sqlite3_stmt *pStmt, int i);

sqlite3_value *AnyOffice_sqlite3_column_value(sqlite3_stmt *pStmt, int i);

#ifndef SQLITE_OMIT_UTF16
const void *AnyOffice_sqlite3_column_text16(sqlite3_stmt *pStmt, int i);
#endif

int AnyOffice_sqlite3_column_type(sqlite3_stmt *pStmt, int i);

int AnyOffice_sqlite3_finalize(sqlite3_stmt *pStmt);

int AnyOffice_sqlite3_reset(sqlite3_stmt *pStmt);

int AnyOffice_sqlite3_create_function(
  sqlite3 *db,
  const char *zFunc,
  int nArg,
  int enc,
  void *p,
  void (*xFunc)(sqlite3_context*,int,sqlite3_value **),
  void (*xStep)(sqlite3_context*,int,sqlite3_value **),
  void (*xFinal)(sqlite3_context*)
);

int AnyOffice_sqlite3_create_function_v2(
  sqlite3 *db,
  const char *zFunc,
  int nArg,
  int enc,
  void *p,
  void (*xFunc)(sqlite3_context*,int,sqlite3_value **),
  void (*xStep)(sqlite3_context*,int,sqlite3_value **),
  void (*xFinal)(sqlite3_context*),
  void (*xDestroy)(void *)
);

#ifndef SQLITE_OMIT_UTF16
int AnyOffice_sqlite3_create_function16(
  sqlite3 *db,
  const void *zFunctionName,
  int nArg,
  int eTextRep,
  void *p,
  void (*xFunc)(sqlite3_context*,int,sqlite3_value**),
  void (*xStep)(sqlite3_context*,int,sqlite3_value**),
  void (*xFinal)(sqlite3_context*)
);
#endif

#ifndef SQLITE_OMIT_DEPRECATED
int AnyOffice_sqlite3_aggregate_count(sqlite3_context *p);
int AnyOffice_sqlite3_expired(sqlite3_stmt *pStmt);
int AnyOffice_sqlite3_transfer_bindings(sqlite3_stmt *pFromStmt, sqlite3_stmt *pToStmt);
int AnyOffice_sqlite3_global_recover(void);
void AnyOffice_sqlite3_thread_cleanup(void);
int AnyOffice_sqlite3_memory_alarm(
  void(*xCallback)(void *pArg, sqlite3_int64 used,int N),
  void *pArg,
  sqlite3_int64 iThreshold
);
#endif

const void *AnyOffice_sqlite3_value_blob(sqlite3_value *pVal);

int AnyOffice_sqlite3_value_bytes(sqlite3_value *pVal);

int AnyOffice_sqlite3_value_bytes16(sqlite3_value *pVal);
double AnyOffice_sqlite3_value_double(sqlite3_value *pVal);

int AnyOffice_sqlite3_value_int(sqlite3_value *pVal);

sqlite_int64 AnyOffice_sqlite3_value_int64(sqlite3_value *pVal);

const unsigned char *AnyOffice_sqlite3_value_text(sqlite3_value *pVal);

#ifndef SQLITE_OMIT_UTF16
const void *AnyOffice_sqlite3_value_text16(sqlite3_value* pVal);
const void *AnyOffice_sqlite3_value_text16be(sqlite3_value *pVal);
const void *AnyOffice_sqlite3_value_text16le(sqlite3_value *pVal);
#endif

int AnyOffice_sqlite3_value_type(sqlite3_value* pVal);

int AnyOffice_sqlite3_value_numeric_type(sqlite3_value *pVal);

void *AnyOffice_sqlite3_aggregate_context(sqlite3_context *p, int nByte);

void *AnyOffice_sqlite3_user_data(sqlite3_context *p);

sqlite3 *AnyOffice_sqlite3_context_db_handle(sqlite3_context *p);

void *AnyOffice_sqlite3_get_auxdata(sqlite3_context *pCtx, int iArg);

void AnyOffice_sqlite3_set_auxdata(
  sqlite3_context *pCtx, 
  int iArg, 
  void *pAux, 
  void (*xDelete)(void*)
);

void AnyOffice_sqlite3_result_blob(
  sqlite3_context *pCtx, 
  const void *z, 
  int n, 
  void (*xDel)(void *)
);


void AnyOffice_sqlite3_result_double(sqlite3_context *pCtx, double rVal);

void AnyOffice_sqlite3_result_error(sqlite3_context *pCtx, const char *z, int n);

#ifndef SQLITE_OMIT_UTF16
 void AnyOffice_sqlite3_result_error16(sqlite3_context *pCtx, const void *z, int n);
#endif

void AnyOffice_sqlite3_result_int64(sqlite3_context *pCtx, sqlite_int64 iVal);

void AnyOffice_sqlite3_result_int(sqlite3_context *pCtx, int iVal);

void AnyOffice_sqlite3_result_null(sqlite3_context *pCtx);

void AnyOffice_sqlite3_result_text(
  sqlite3_context *pCtx, 
  const char *z, 
  int n,
  void (*xDel)(void *)
);

#ifndef SQLITE_OMIT_UTF16
void AnyOffice_sqlite3_result_text16(
  sqlite3_context *pCtx, 
  const void *z, 
  int n, 
  void (*xDel)(void *)
);

void AnyOffice_sqlite3_result_text16be(
  sqlite3_context *pCtx, 
  const void *z, 
  int n, 
  void (*xDel)(void *)
);

void AnyOffice_sqlite3_result_text16le(
  sqlite3_context *pCtx, 
  const void *z, 
  int n, 
  void (*xDel)(void *)
);
#endif

void AnyOffice_sqlite3_result_value(sqlite3_context *pCtx, sqlite3_value *pValue);

void AnyOffice_sqlite3_result_zeroblob(sqlite3_context *pCtx, int n);

void AnyOffice_sqlite3_result_error_code(sqlite3_context *pCtx, int errCode);

void AnyOffice_sqlite3_result_error_toobig(sqlite3_context *pCtx);

void AnyOffice_sqlite3_result_error_nomem(sqlite3_context *pCtx);

int AnyOffice_sqlite3_create_collation(
  sqlite3* db, 
  const char *zName, 
  int enc, 
  void* pCtx,
  int(*xCompare)(void*,int,const void*,int,const void*)
);

int AnyOffice_sqlite3_create_collation_v2(
  sqlite3* db, 
  const char *zName, 
  int enc, 
  void* pCtx,
  int(*xCompare)(void*,int,const void*,int,const void*),
  void(*xDel)(void*)
);

#ifndef SQLITE_OMIT_UTF16
int AnyOffice_sqlite3_create_collation16(
  sqlite3* db, 
  const void *zName,
  int enc, 
  void* pCtx,
  int(*xCompare)(void*,int,const void*,int,const void*)
);
#endif

int AnyOffice_sqlite3_collation_needed(
  sqlite3 *db, 
  void *pCollNeededArg, 
  void(*xCollNeeded)(void*,sqlite3*,int eTextRep,const char*)
);


#ifndef SQLITE_OMIT_UTF16
int AnyOffice_sqlite3_collation_needed16(
  sqlite3 *db, 
  void *pCollNeededArg, 
  void(*xCollNeeded16)(void*,sqlite3*,int eTextRep,const void*)
);
#endif

int AnyOffice_sqlite3_sleep(int ms);

int AnyOffice_sqlite3_get_autocommit(sqlite3 *db);

sqlite3 *AnyOffice_sqlite3_db_handle(sqlite3_stmt *pStmt);

const char *AnyOffice_sqlite3_db_filename(sqlite3 *db, const char *zDbName);

int AnyOffice_sqlite3_db_readonly(sqlite3 *db, const char *zDbName);

sqlite3_stmt *AnyOffice_sqlite3_next_stmt(sqlite3 *pDb, sqlite3_stmt *pStmt);

void *AnyOffice_sqlite3_commit_hook(
  sqlite3 *db,              /* Attach the hook to this database */
  int (*xCallback)(void*),  /* Function to invoke on each commit */
  void *pArg                /* Argument to the function */
);

void *AnyOffice_sqlite3_rollback_hook(
  sqlite3 *db,              /* Attach the hook to this database */
  void (*xCallback)(void*), /* Callback function */
  void *pArg                /* Argument to the function */
);


void *AnyOffice_sqlite3_update_hook(
  sqlite3 *db,              /* Attach the hook to this database */
  void (*xCallback)(void*,int,char const *,char const *,sqlite_int64),
  void *pArg                /* Argument to the function */
);

int AnyOffice_sqlite3_enable_shared_cache(int enable);

int AnyOffice_sqlite3_release_memory(int n);

int AnyOffice_sqlite3_db_release_memory(sqlite3 *db);


sqlite3_int64 AnyOffice_sqlite3_soft_heap_limit64(sqlite3_int64 n);

void AnyOffice_sqlite3_soft_heap_limit(int n);

int AnyOffice_sqlite3_load_extension(
  sqlite3 *db,          /* Load the extension into this database connection */
  const char *zFile,    /* Name of the shared library containing extension */
  const char *zProc,    /* Entry point.  Use "sqlite3_extension_init" if 0 */
  char **pzErrMsg       /* Put error message here if not 0 */
);

int AnyOffice_sqlite3_enable_load_extension(sqlite3 *db, int onoff);

int AnyOffice_sqlite3_auto_extension(void (*xInit)(void));

void AnyOffice_sqlite3_reset_auto_extension(void);

int AnyOffice_sqlite3_create_module(
  sqlite3 *db,                    /* Database in which module is registered */
  const char *zName,              /* Name assigned to this module */
  const sqlite3_module *pModule,  /* The definition of the module */
  void *pAux                      /* Context pointer for xCreate/xConnect */
);

int AnyOffice_sqlite3_create_module_v2(
  sqlite3 *db,                    /* Database in which module is registered */
  const char *zName,              /* Name assigned to this module */
  const sqlite3_module *pModule,  /* The definition of the module */
  void *pAux,                     /* Context pointer for xCreate/xConnect */
  void (*xDestroy)(void *)        /* Module destructor function */
);

int AnyOffice_sqlite3_declare_vtab(sqlite3 *db, const char *zCreateTable);

int AnyOffice_sqlite3_overload_function(
  sqlite3 *db,
  const char *zName,
  int nArg
);

int AnyOffice_sqlite3_blob_open(
  sqlite3* db,            /* The database connection */
  const char *zDb,        /* The attached database containing the blob */
  const char *zTable,     /* The table containing the blob */
  const char *zColumn,    /* The column containing the blob */
  sqlite_int64 iRow,      /* The row containing the glob */
  int flags,              /* True -> read/write access, false -> read-only */
  sqlite3_blob **ppBlob   /* Handle for accessing the blob returned here */
);

int AnyOffice_sqlite3_blob_reopen(sqlite3_blob *pBlob, sqlite3_int64 iRow);

int AnyOffice_sqlite3_blob_close(sqlite3_blob *pBlob);

int AnyOffice_sqlite3_blob_bytes(sqlite3_blob *pBlob);

int AnyOffice_sqlite3_blob_read(sqlite3_blob *pBlob, void *z, int n, int iOffset);

int AnyOffice_sqlite3_blob_write(sqlite3_blob *pBlob, const void *z, int n, int iOffset);

sqlite3_vfs *AnyOffice_sqlite3_vfs_find(const char *zVfs);

int AnyOffice_sqlite3_vfs_register(sqlite3_vfs *pVfs, int makeDflt);

int AnyOffice_sqlite3_vfs_unregister(sqlite3_vfs *pVfs);

sqlite3_mutex *AnyOffice_sqlite3_mutex_alloc(int id);

void AnyOffice_sqlite3_mutex_free(sqlite3_mutex *p);

void AnyOffice_sqlite3_mutex_enter(sqlite3_mutex *p);

void AnyOffice_sqlite3_mutex_leave(sqlite3_mutex *p);

int AnyOffice_sqlite3_mutex_try(sqlite3_mutex *p);

sqlite3_mutex *AnyOffice_sqlite3_db_mutex(sqlite3 *db);

int AnyOffice_sqlite3_file_control(sqlite3 *db, const char *zDbName, int op, void *pArg);

int AnyOffice_sqlite3_test_control(int op, ...);

int AnyOffice_sqlite3_status(int op, int *pCurrent, int *pHighwater, int resetFlag);

int AnyOffice_sqlite3_db_status(
  sqlite3 *db,          /* The database connection whose status is desired */
  int op,               /* Status verb */
  int *pCurrent,        /* Write current value here */
  int *pHighwater,      /* Write high-water mark here */
  int resetFlag         /* Reset high-water mark if true */
);

int AnyOffice_sqlite3_stmt_status(sqlite3_stmt *pStmt, int op, int resetFlag);

sqlite3_backup *AnyOffice_sqlite3_backup_init(
  sqlite3* pDestDb,                     /* Database to write to */
  const char *zDestDb,                  /* Name of database within pDestDb */
  sqlite3* pSrcDb,                      /* Database connection to read from */
  const char *zSrcDb                    /* Name of database within pSrcDb */
);

int AnyOffice_sqlite3_backup_step(sqlite3_backup *p, int nPage);

int AnyOffice_sqlite3_backup_finish(sqlite3_backup *p);

int AnyOffice_sqlite3_backup_remaining(sqlite3_backup *p);


int AnyOffice_sqlite3_backup_pagecount(sqlite3_backup *p);

int AnyOffice_sqlite3_stricmp(const char *zLeft, const char *zRight);

int AnyOffice_sqlite3_strnicmp(const char *zLeft, const char *zRight, int N);

int AnyOffice_sqlite3_strglob(const char *zGlobPattern, const char *zString);

void AnyOffice_sqlite3_log(int iErrCode, const char *zFormat, ...);

void *AnyOffice_sqlite3_wal_hook(
  sqlite3 *db,                    /* Attach the hook to this db handle */
  int(*xCallback)(void *, sqlite3*, const char*, int),
  void *pArg                      /* First argument passed to xCallback() */
);

int AnyOffice_sqlite3_wal_autocheckpoint(sqlite3 *db, int nFrame);

int AnyOffice_sqlite3_wal_checkpoint(sqlite3 *db, const char *zDb);

int AnyOffice_sqlite3_wal_checkpoint_v2(
  sqlite3 *db,                    /* Database handle */
  const char *zDb,                /* Name of attached database (or NULL) */
  int eMode,                      /* SQLITE_CHECKPOINT_* value */
  int *pnLog,                     /* OUT: Size of WAL log in frames */
  int *pnCkpt                     /* OUT: Total number of frames checkpointed */
);

int AnyOffice_sqlite3_vtab_config(sqlite3 *db, int op, ...);

int AnyOffice_sqlite3_vtab_on_conflict(sqlite3 *db);

int AnyOffice_sqlite3_rekey_v2(sqlite3 *db, const char *zDbName, const void *pKey, int nKey);

int AnyOffice_sqlite3_key(sqlite3 *db, const void *pKey, int nKey);

SQLITE_API int sqlite3_open_rekey(
          const char *filename,   /* Database filename (UTF-8) */
          int flags,              /* Flags */
          const char *zVfs,       /* Name of VFS module to use */
          const char* username    /* User name */
          );
   

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

