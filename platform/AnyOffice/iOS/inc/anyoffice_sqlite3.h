/******************************************************************************

  Copyright (C), 2001-2011, Huawei Tech. Co., Ltd.

 ******************************************************************************
  File Name     : anyoffice_sqlite3.h
  Version       : Initial Draft
  Author        : zhangtailei 00218689
  Created       : 2015/3/27
  Last Modified :
  Description   : sqlite3对外头文件
  Function List :
  History       :
  1.Date        : 2015/3/27
    Author      : zhangtailei 00218689
    Modification: Created file

******************************************************************************/
#ifndef _SQLITE3_H_ /*  */
#define _SQLITE3_H_



#include "AnyOffice_sqlite3_impl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define SQLITE_ACCESS_EXISTS    0
#define SQLITE_ACCESS_READWRITE 1   /* Used by PRAGMA temp_store_directory */
#define SQLITE_ACCESS_READ      2   /* Unused */


#define SQLITE_FCNTL_LOCKSTATE               1
#define SQLITE_GET_LOCKPROXYFILE             2
#define SQLITE_SET_LOCKPROXYFILE             3
#define SQLITE_LAST_ERRNO                    4
#define SQLITE_FCNTL_SIZE_HINT               5
#define SQLITE_FCNTL_CHUNK_SIZE              6
#define SQLITE_FCNTL_FILE_POINTER            7
#define SQLITE_FCNTL_SYNC_OMITTED            8
#define SQLITE_FCNTL_WIN32_AV_RETRY          9
#define SQLITE_FCNTL_PERSIST_WAL            10
#define SQLITE_FCNTL_OVERWRITE              11
#define SQLITE_FCNTL_VFSNAME                12
#define SQLITE_FCNTL_POWERSAFE_OVERWRITE    13
#define SQLITE_FCNTL_PRAGMA                 14
#define SQLITE_FCNTL_BUSYHANDLER            15
#define SQLITE_FCNTL_TEMPFILENAME           16
#define SQLITE_FCNTL_MMAP_SIZE              18

#define SQLITE_DBSTATUS_LOOKASIDE_USED       0
#define SQLITE_DBSTATUS_CACHE_USED           1
#define SQLITE_DBSTATUS_SCHEMA_USED          2
#define SQLITE_DBSTATUS_STMT_USED            3
#define SQLITE_DBSTATUS_LOOKASIDE_HIT        4
#define SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE  5
#define SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL  6
#define SQLITE_DBSTATUS_CACHE_HIT            7
#define SQLITE_DBSTATUS_CACHE_MISS           8
#define SQLITE_DBSTATUS_CACHE_WRITE          9
#define SQLITE_DBSTATUS_MAX                  9   /* Largest defined DBSTATUS */

#define SQLITE_STMTSTATUS_FULLSCAN_STEP     1
#define SQLITE_STMTSTATUS_SORT              2
#define SQLITE_STMTSTATUS_AUTOINDEX         3

#define SQLITE_VTAB_CONSTRAINT_SUPPORT 1

#define SQLITE_CHECKPOINT_PASSIVE 0
#define SQLITE_CHECKPOINT_FULL    1
#define SQLITE_CHECKPOINT_RESTART 2

#define SQLITE_OK           0   /* Successful result */
/* beginning-of-error-codes */
#define SQLITE_ERROR        1   /* SQL error or missing database */
#define SQLITE_INTERNAL     2   /* Internal logic error in SQLite */
#define SQLITE_PERM         3   /* Access permission denied */
#define SQLITE_ABORT        4   /* Callback routine requested an abort */
#define SQLITE_BUSY         5   /* The database file is locked */
#define SQLITE_LOCKED       6   /* A table in the database is locked */
#define SQLITE_NOMEM        7   /* A malloc() failed */
#define SQLITE_READONLY     8   /* Attempt to write a readonly database */
#define SQLITE_INTERRUPT    9   /* Operation terminated by sqlite3_interrupt()*/
#define SQLITE_IOERR       10   /* Some kind of disk I/O error occurred */
#define SQLITE_CORRUPT     11   /* The database disk image is malformed */
#define SQLITE_NOTFOUND    12   /* Unknown opcode in sqlite3_file_control() */
#define SQLITE_FULL        13   /* Insertion failed because database is full */
#define SQLITE_CANTOPEN    14   /* Unable to open the database file */
#define SQLITE_PROTOCOL    15   /* Database lock protocol error */
#define SQLITE_EMPTY       16   /* Database is empty */
#define SQLITE_SCHEMA      17   /* The database schema changed */
#define SQLITE_TOOBIG      18   /* String or BLOB exceeds size limit */
#define SQLITE_CONSTRAINT  19   /* Abort due to constraint violation */
#define SQLITE_MISMATCH    20   /* Data type mismatch */
#define SQLITE_MISUSE      21   /* Library used incorrectly */
#define SQLITE_NOLFS       22   /* Uses OS features not supported on host */
#define SQLITE_AUTH        23   /* Authorization denied */
#define SQLITE_FORMAT      24   /* Auxiliary database format error */
#define SQLITE_RANGE       25   /* 2nd parameter to sqlite3_bind out of range */
#define SQLITE_NOTADB      26   /* File opened that is not a database file */
#define SQLITE_NOTICE      27   /* Notifications from sqlite3_log() */
#define SQLITE_WARNING     28   /* Warnings from sqlite3_log() */
#define SQLITE_ROW         100  /* sqlite3_step() has another row ready */
#define SQLITE_DONE        101  /* sqlite3_step() has finished executing */

#define SQLITE_OPEN_READONLY         0x00000001  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_READWRITE        0x00000002  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_CREATE           0x00000004  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_DELETEONCLOSE    0x00000008  /* VFS only */
#define SQLITE_OPEN_EXCLUSIVE        0x00000010  /* VFS only */
#define SQLITE_OPEN_AUTOPROXY        0x00000020  /* VFS only */
#define SQLITE_OPEN_URI              0x00000040  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_MEMORY           0x00000080  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_MAIN_DB          0x00000100  /* VFS only */
#define SQLITE_OPEN_TEMP_DB          0x00000200  /* VFS only */
#define SQLITE_OPEN_TRANSIENT_DB     0x00000400  /* VFS only */
#define SQLITE_OPEN_MAIN_JOURNAL     0x00000800  /* VFS only */
#define SQLITE_OPEN_TEMP_JOURNAL     0x00001000  /* VFS only */
#define SQLITE_OPEN_SUBJOURNAL       0x00002000  /* VFS only */
#define SQLITE_OPEN_MASTER_JOURNAL   0x00004000  /* VFS only */
#define SQLITE_OPEN_NOMUTEX          0x00008000  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_FULLMUTEX        0x00010000  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_SHAREDCACHE      0x00020000  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_PRIVATECACHE     0x00040000  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_WAL              0x00080000  /* VFS only */

#define SQLITE_INTEGER  1
#define SQLITE_FLOAT    2
#define SQLITE_BLOB     4
#define SQLITE_NULL     5
#ifdef SQLITE_TEXT
# undef SQLITE_TEXT
#else
# define SQLITE_TEXT     3
#endif
#define SQLITE3_TEXT     3

#define SQLITE_UTF8           1
#define SQLITE_UTF16LE        2
#define SQLITE_UTF16BE        3
#define SQLITE_UTF16          4    /* Use native byte order */
#define SQLITE_ANY            5    /* sqlite3_create_function only */
#define SQLITE_UTF16_ALIGNED  8    /* sqlite3_create_collation only */

#define SQLITE_MUTEX_FAST             0
#define SQLITE_MUTEX_RECURSIVE        1
#define SQLITE_MUTEX_STATIC_MASTER    2
#define SQLITE_MUTEX_STATIC_MEM       3  /* sqlite3_malloc() */
#define SQLITE_MUTEX_STATIC_MEM2      4  /* NOT USED */
#define SQLITE_MUTEX_STATIC_OPEN      4  /* sqlite3BtreeOpen() */
#define SQLITE_MUTEX_STATIC_PRNG      5  /* sqlite3_random() */
#define SQLITE_MUTEX_STATIC_LRU       6  /* lru page list */
#define SQLITE_MUTEX_STATIC_LRU2      7  /* NOT USED */
#define SQLITE_MUTEX_STATIC_PMEM      7  /* sqlite3PageMalloc() */

#define SQLITE_SHM_UNLOCK       1
#define SQLITE_SHM_LOCK         2
#define SQLITE_SHM_SHARED       4
#define SQLITE_SHM_EXCLUSIVE    8

/*
** CAPI3REF: Maximum xShmLock index
**
** The xShmLock method on [sqlite3_io_methods] may use values
** between 0 and this upper bound as its "offset" argument.
** The SQLite core will never attempt to acquire or release a
** lock outside of this range
*/
#define SQLITE_SHM_NLOCK        8

#define SQLITE_ROLLBACK 1
/* #define SQLITE_IGNORE 2 // Also used by sqlite3_authorizer() callback */
#define SQLITE_FAIL     3
/* #define SQLITE_ABORT 4  // Also an error code */
#define SQLITE_REPLACE  5

#define SQLITE_STATUS_MEMORY_USED          0
#define SQLITE_STATUS_PAGECACHE_USED       1
#define SQLITE_STATUS_PAGECACHE_OVERFLOW   2
#define SQLITE_STATUS_SCRATCH_USED         3
#define SQLITE_STATUS_SCRATCH_OVERFLOW     4
#define SQLITE_STATUS_MALLOC_SIZE          5
#define SQLITE_STATUS_PARSER_STACK         6
#define SQLITE_STATUS_PAGECACHE_SIZE       7
#define SQLITE_STATUS_SCRATCH_SIZE         8
#define SQLITE_STATUS_MALLOC_COUNT         9


#define SQLITE_CONFIG_SINGLETHREAD  1  /* nil */
#define SQLITE_CONFIG_MULTITHREAD   2  /* nil */
#define SQLITE_CONFIG_SERIALIZED    3  /* nil */
#define SQLITE_CONFIG_MALLOC        4  /* sqlite3_mem_methods* */
#define SQLITE_CONFIG_GETMALLOC     5  /* sqlite3_mem_methods* */
#define SQLITE_CONFIG_SCRATCH       6  /* void*, int sz, int N */
#define SQLITE_CONFIG_PAGECACHE     7  /* void*, int sz, int N */
#define SQLITE_CONFIG_HEAP          8  /* void*, int nByte, int min */
#define SQLITE_CONFIG_MEMSTATUS     9  /* boolean */
#define SQLITE_CONFIG_MUTEX        10  /* sqlite3_mutex_methods* */
#define SQLITE_CONFIG_GETMUTEX     11  /* sqlite3_mutex_methods* */
/* previously SQLITE_CONFIG_CHUNKALLOC 12 which is now unused. */ 
#define SQLITE_CONFIG_LOOKASIDE    13  /* int int */
#define SQLITE_CONFIG_PCACHE       14  /* no-op */
#define SQLITE_CONFIG_GETPCACHE    15  /* no-op */
#define SQLITE_CONFIG_LOG          16  /* xFunc, void* */
#define SQLITE_CONFIG_URI          17  /* int */
#define SQLITE_CONFIG_PCACHE2      18  /* sqlite3_pcache_methods2* */
#define SQLITE_CONFIG_GETPCACHE2   19  /* sqlite3_pcache_methods2* */
#define SQLITE_CONFIG_COVERING_INDEX_SCAN 20  /* int */
#define SQLITE_CONFIG_SQLLOG       21  /* xSqllog, void* */
#define SQLITE_CONFIG_MMAP_SIZE    22  /* sqlite3_int64, sqlite3_int64 */

#define SQLITE_CREATE_INDEX          1   /* Index Name      Table Name      */
#define SQLITE_CREATE_TABLE          2   /* Table Name      NULL            */
#define SQLITE_CREATE_TEMP_INDEX     3   /* Index Name      Table Name      */
#define SQLITE_CREATE_TEMP_TABLE     4   /* Table Name      NULL            */
#define SQLITE_CREATE_TEMP_TRIGGER   5   /* Trigger Name    Table Name      */
#define SQLITE_CREATE_TEMP_VIEW      6   /* View Name       NULL            */
#define SQLITE_CREATE_TRIGGER        7   /* Trigger Name    Table Name      */
#define SQLITE_CREATE_VIEW           8   /* View Name       NULL            */
#define SQLITE_DELETE                9   /* Table Name      NULL            */
#define SQLITE_DROP_INDEX           10   /* Index Name      Table Name      */
#define SQLITE_DROP_TABLE           11   /* Table Name      NULL            */
#define SQLITE_DROP_TEMP_INDEX      12   /* Index Name      Table Name      */
#define SQLITE_DROP_TEMP_TABLE      13   /* Table Name      NULL            */
#define SQLITE_DROP_TEMP_TRIGGER    14   /* Trigger Name    Table Name      */
#define SQLITE_DROP_TEMP_VIEW       15   /* View Name       NULL            */
#define SQLITE_DROP_TRIGGER         16   /* Trigger Name    Table Name      */
#define SQLITE_DROP_VIEW            17   /* View Name       NULL            */
#define SQLITE_INSERT               18   /* Table Name      NULL            */
#define SQLITE_PRAGMA               19   /* Pragma Name     1st arg or NULL */
#define SQLITE_READ                 20   /* Table Name      Column Name     */
#define SQLITE_SELECT               21   /* NULL            NULL            */
#define SQLITE_TRANSACTION          22   /* Operation       NULL            */
#define SQLITE_UPDATE               23   /* Table Name      Column Name     */
#define SQLITE_ATTACH               24   /* Filename        NULL            */
#define SQLITE_DETACH               25   /* Database Name   NULL            */
#define SQLITE_ALTER_TABLE          26   /* Database Name   Table Name      */
#define SQLITE_REINDEX              27   /* Index Name      NULL            */
#define SQLITE_ANALYZE              28   /* Table Name      NULL            */
#define SQLITE_CREATE_VTABLE        29   /* Table Name      Module Name     */
#define SQLITE_DROP_VTABLE          30   /* Table Name      Module Name     */
#define SQLITE_FUNCTION             31   /* NULL            Function Name   */
#define SQLITE_SAVEPOINT            32   /* Operation       Savepoint Name  */
#define SQLITE_COPY                  0   /* No longer used */


#define SQLITE_LIMIT_LENGTH                    0
#define SQLITE_LIMIT_SQL_LENGTH                1
#define SQLITE_LIMIT_COLUMN                    2
#define SQLITE_LIMIT_EXPR_DEPTH                3
#define SQLITE_LIMIT_COMPOUND_SELECT           4
#define SQLITE_LIMIT_VDBE_OP                   5
#define SQLITE_LIMIT_FUNCTION_ARG              6
#define SQLITE_LIMIT_ATTACHED                  7
#define SQLITE_LIMIT_LIKE_PATTERN_LENGTH       8
#define SQLITE_LIMIT_VARIABLE_NUMBER           9
#define SQLITE_LIMIT_TRIGGER_DEPTH            10


#define SQLITE_INDEX_CONSTRAINT_EQ    2
#define SQLITE_INDEX_CONSTRAINT_GT    4
#define SQLITE_INDEX_CONSTRAINT_LE    8
#define SQLITE_INDEX_CONSTRAINT_LT    16
#define SQLITE_INDEX_CONSTRAINT_GE    32
#define SQLITE_INDEX_CONSTRAINT_MATCH 64

#define SQLITE_TESTCTRL_FIRST                    5
#define SQLITE_TESTCTRL_PRNG_SAVE                5
#define SQLITE_TESTCTRL_PRNG_RESTORE             6
#define SQLITE_TESTCTRL_PRNG_RESET               7
#define SQLITE_TESTCTRL_BITVEC_TEST              8
#define SQLITE_TESTCTRL_FAULT_INSTALL            9
#define SQLITE_TESTCTRL_BENIGN_MALLOC_HOOKS     10
#define SQLITE_TESTCTRL_PENDING_BYTE            11
#define SQLITE_TESTCTRL_ASSERT                  12
#define SQLITE_TESTCTRL_ALWAYS                  13
#define SQLITE_TESTCTRL_RESERVE                 14
#define SQLITE_TESTCTRL_OPTIMIZATIONS           15
#define SQLITE_TESTCTRL_ISKEYWORD               16
#define SQLITE_TESTCTRL_SCRATCHMALLOC           17
#define SQLITE_TESTCTRL_LOCALTIME_FAULT         18
#define SQLITE_TESTCTRL_EXPLAIN_STMT            19
#define SQLITE_TESTCTRL_LAST                    19
/*****************************************************************
                    sqlite3接口封装
*****************************************************************/

// TODO:
//#define  sqlite3_temp_directory          AnyOffice_sqlite3_temp_directory
//#define  sqlite3_data_directory          AnyOffice_sqlite3_data_directory 
#define  sqlite3_aggregate_context      AnyOffice_sqlite3_aggregate_context

#define  sqlite3_auto_extension             AnyOffice_sqlite3_auto_extension
#define  sqlite3_backup_finish             AnyOffice_sqlite3_backup_finish
#define  sqlite3_backup_init             AnyOffice_sqlite3_backup_init
#define  sqlite3_backup_pagecount             AnyOffice_sqlite3_backup_pagecount
#define  sqlite3_backup_remaining     AnyOffice_sqlite3_backup_remaining
#define  sqlite3_backup_step             AnyOffice_sqlite3_backup_step
#define  sqlite3_bind_blob             AnyOffice_sqlite3_bind_blob
#define  sqlite3_bind_double             AnyOffice_sqlite3_bind_double
#define  sqlite3_bind_int             AnyOffice_sqlite3_bind_int
#define  sqlite3_bind_int64             AnyOffice_sqlite3_bind_int64
#define  sqlite3_bind_null             AnyOffice_sqlite3_bind_null
#define  sqlite3_bind_parameter_count             AnyOffice_sqlite3_bind_parameter_count
#define  sqlite3_bind_parameter_index             AnyOffice_sqlite3_bind_parameter_index
#define  sqlite3_bind_parameter_name             AnyOffice_sqlite3_bind_parameter_name
#define  sqlite3_bind_text             AnyOffice_sqlite3_bind_text
#define  sqlite3_bind_value             AnyOffice_sqlite3_bind_value
#define  sqlite3_bind_zeroblob             AnyOffice_sqlite3_bind_zeroblob
#define  sqlite3_blob_bytes             AnyOffice_sqlite3_blob_bytes
#define  sqlite3_blob_close             AnyOffice_sqlite3_blob_close
#define  sqlite3_blob_open             AnyOffice_sqlite3_blob_open
#define  sqlite3_blob_read             AnyOffice_sqlite3_blob_read
#define  sqlite3_blob_reopen             AnyOffice_sqlite3_blob_reopen
#define  sqlite3_blob_write             AnyOffice_sqlite3_blob_write
#define  sqlite3_busy_handler             AnyOffice_sqlite3_busy_handler
#define  sqlite3_busy_timeout             AnyOffice_sqlite3_busy_timeout
#define  sqlite3_changes             AnyOffice_sqlite3_changes
#define  sqlite3_clear_bindings             AnyOffice_sqlite3_clear_bindings
#define  sqlite3_close             AnyOffice_sqlite3_close
#define  sqlite3_close_v2             AnyOffice_sqlite3_close_v2
#define  sqlite3_collation_needed             AnyOffice_sqlite3_collation_needed
#define  sqlite3_column_blob             AnyOffice_sqlite3_column_blob
#define  sqlite3_column_bytes             AnyOffice_sqlite3_column_bytes
#define  sqlite3_column_bytes16             AnyOffice_sqlite3_column_bytes16
#define  sqlite3_column_count             AnyOffice_sqlite3_column_count
#define  sqlite3_column_double             AnyOffice_sqlite3_column_double
#define  sqlite3_column_int             AnyOffice_sqlite3_column_int
#define  sqlite3_column_int64             AnyOffice_sqlite3_column_int64
#define  sqlite3_column_name             AnyOffice_sqlite3_column_name
#define  sqlite3_column_text      AnyOffice_sqlite3_column_text
#define  sqlite3_column_type             AnyOffice_sqlite3_column_type
#define  sqlite3_column_value             AnyOffice_sqlite3_column_value
#define  sqlite3_commit_hook             AnyOffice_sqlite3_commit_hook
#define  sqlite3_compileoption_get             AnyOffice_sqlite3_compileoption_get
#define  sqlite3_compileoption_used             AnyOffice_sqlite3_compileoption_used
#define  sqlite3_complete             AnyOffice_sqlite3_complete
#define  sqlite3_config             AnyOffice_sqlite3_config
#define  sqlite3_context_db_handle             AnyOffice_sqlite3_context_db_handle
#define  sqlite3_create_collation             AnyOffice_sqlite3_create_collation

#define  sqlite3_create_collation_v2             AnyOffice_sqlite3_create_collation_v2
#define  sqlite3_create_function             AnyOffice_sqlite3_create_function
#define  sqlite3_create_function_v2             AnyOffice_sqlite3_create_function_v2
#define  sqlite3_create_module             AnyOffice_sqlite3_create_module
#define  sqlite3_create_module_v2             AnyOffice_sqlite3_create_module_v2
#define  sqlite3_data_count             AnyOffice_sqlite3_data_count
#define  sqlite3_db_config             AnyOffice_sqlite3_db_config
#define  sqlite3_db_filename             AnyOffice_sqlite3_db_filename
#define  sqlite3_db_handle             AnyOffice_sqlite3_db_handle
#define  sqlite3_db_mutex             AnyOffice_sqlite3_db_mutex
#define  sqlite3_db_readonly             AnyOffice_sqlite3_db_readonly
#define  sqlite3_db_release_memory             AnyOffice_sqlite3_db_release_memory
#define  sqlite3_db_status             AnyOffice_sqlite3_db_status
#define  sqlite3_declare_vtab             AnyOffice_sqlite3_declare_vtab
#define  sqlite3_enable_load_extension             AnyOffice_sqlite3_enable_load_extension
#define  sqlite3_enable_shared_cache             AnyOffice_sqlite3_enable_shared_cache
#define  sqlite3_errcode             AnyOffice_sqlite3_errcode
#define  sqlite3_errmsg             AnyOffice_sqlite3_errmsg

#define  sqlite3_errstr             AnyOffice_sqlite3_errstr
#define  sqlite3_exec             AnyOffice_sqlite3_exec

#define  sqlite3_extended_errcode             AnyOffice_sqlite3_extended_errcode
#define  sqlite3_extended_result_codes             AnyOffice_sqlite3_extended_result_codes
#define  sqlite3_file_control             AnyOffice_sqlite3_file_control
#define  sqlite3_finalize             AnyOffice_sqlite3_finalize
#define  sqlite3_free             AnyOffice_sqlite3_free
#define  sqlite3_free_table             AnyOffice_sqlite3_free_table
#define  sqlite3_get_autocommit             AnyOffice_sqlite3_get_autocommit
#define  sqlite3_get_auxdata             AnyOffice_sqlite3_get_auxdata
#define  sqlite3_get_table             AnyOffice_sqlite3_get_table

#define  sqlite3_initialize             AnyOffice_sqlite3_initialize
#define  sqlite3_interrupt             AnyOffice_sqlite3_interrupt
#define  sqlite3_last_insert_rowid             AnyOffice_sqlite3_last_insert_rowid
#define  sqlite3_libversion             AnyOffice_sqlite3_libversion
#define  sqlite3_libversion_number             AnyOffice_sqlite3_libversion_number
#define  sqlite3_limit             AnyOffice_sqlite3_limit
#define  sqlite3_load_extension             AnyOffice_sqlite3_load_extension
#define  sqlite3_log             AnyOffice_sqlite3_log
#define  sqlite3_malloc             AnyOffice_sqlite3_malloc

#define  sqlite3_memory_highwater             AnyOffice_sqlite3_memory_highwater
#define  sqlite3_memory_used             AnyOffice_sqlite3_memory_used
#define  sqlite3_mprintf             AnyOffice_sqlite3_mprintf
#define  sqlite3_mutex_alloc             AnyOffice_sqlite3_mutex_alloc
#define  sqlite3_mutex_enter             AnyOffice_sqlite3_mutex_enter
#define  sqlite3_mutex_free             AnyOffice_sqlite3_mutex_free
#define  sqlite3_mutex_leave             AnyOffice_sqlite3_mutex_leave
#define  sqlite3_mutex_try            AnyOffice_sqlite3_mutex_try
#define  sqlite3_next_stmt             AnyOffice_sqlite3_next_stmt
#define  sqlite3_open             AnyOffice_sqlite3_open
#define  sqlite3_open_s             AnyOffice_sqlite3_open_s

#define  sqlite3_open_v2             AnyOffice_sqlite3_open_v2
#define  sqlite3_os_end             AnyOffice_sqlite3_os_end
#define  sqlite3_os_init             AnyOffice_sqlite3_os_init
#define  sqlite3_overload_function             AnyOffice_sqlite3_overload_function
#define  sqlite3_prepare             AnyOffice_sqlite3_prepare
#define  sqlite3_prepare16_v2             AnyOffice_sqlite3_prepare16_v2
#define  sqlite3_prepare_v2             AnyOffice_sqlite3_prepare_v2
#define  sqlite3_profile              AnyOffice_sqlite3_profile
#define  sqlite3_progress_handler            AnyOffice_sqlite3_progress_handler
#define  sqlite3_randomness     AnyOffice_sqlite3_randomness
#define  sqlite3_realloc             AnyOffice_sqlite3_realloc
#define  sqlite3_release_memory            AnyOffice_sqlite3_release_memory
#define  sqlite3_reset             AnyOffice_sqlite3_reset
#define  sqlite3_reset_auto_extension             AnyOffice_sqlite3_reset_auto_extension
#define  sqlite3_result_blob             AnyOffice_sqlite3_result_blob
#define  sqlite3_result_double             AnyOffice_sqlite3_result_double
#define  sqlite3_result_error             AnyOffice_sqlite3_result_error

#define  sqlite3_result_error_code             AnyOffice_sqlite3_result_error_code
#define  sqlite3_result_error_nomem             AnyOffice_sqlite3_result_error_nomem
#define  sqlite3_result_error_toobig             AnyOffice_sqlite3_result_error_toobig
#define  sqlite3_result_int             AnyOffice_sqlite3_result_int
#define  sqlite3_result_int64             AnyOffice_sqlite3_result_int64
#define  sqlite3_result_null             AnyOffice_sqlite3_result_null
#define  sqlite3_result_text             AnyOffice_sqlite3_result_text

#define  sqlite3_result_value             AnyOffice_sqlite3_result_value
#define  sqlite3_result_zeroblob             AnyOffice_sqlite3_result_zeroblob
#define  sqlite3_rollback_hook             AnyOffice_sqlite3_rollback_hook
#define  sqlite3_set_authorizer             AnyOffice_sqlite3_set_authorizer
#define  sqlite3_set_auxdata             AnyOffice_sqlite3_set_auxdata
#define  sqlite3_shutdown             AnyOffice_sqlite3_shutdown
#define  sqlite3_sleep             AnyOffice_sqlite3_sleep
#define  sqlite3_snprintf             AnyOffice_sqlite3_snprintf
#define  sqlite3_soft_heap_limit             AnyOffice_sqlite3_soft_heap_limit
#define  sqlite3_soft_heap_limit64             AnyOffice_sqlite3_soft_heap_limit64
#define  sqlite3_sourceid             AnyOffice_sqlite3_sourceid
#define  sqlite3_sql             AnyOffice_sqlite3_sql
#define  sqlite3_status             AnyOffice_sqlite3_status
#define  sqlite3_step             AnyOffice_sqlite3_step
#define  sqlite3_stmt_busy             AnyOffice_sqlite3_stmt_busy
#define  sqlite3_stmt_readonly             AnyOffice_sqlite3_stmt_readonly
#define  sqlite3_stmt_status             AnyOffice_sqlite3_stmt_status
#define  sqlite3_strglob            AnyOffice_sqlite3_strglob
#define  sqlite3_stricmp             AnyOffice_sqlite3_stricmp
#define  sqlite3_strnicmp             AnyOffice_sqlite3_strnicmp
#define  sqlite3_test_control             AnyOffice_sqlite3_test_control
#define  sqlite3_threadsafe             AnyOffice_sqlite3_threadsafe

#define  sqlite3_total_changes             AnyOffice_sqlite3_total_changes
#define  sqlite3_trace             AnyOffice_sqlite3_trace

#define  sqlite3_update_hook             AnyOffice_sqlite3_update_hook
#define  sqlite3_uri_boolean             AnyOffice_sqlite3_uri_boolean
#define  sqlite3_uri_int64             AnyOffice_sqlite3_uri_int64
#define  sqlite3_uri_parameter             AnyOffice_sqlite3_uri_parameter
#define  sqlite3_user_data      AnyOffice_sqlite3_user_data
#define  sqlite3_value_blob             AnyOffice_sqlite3_value_blob
#define  sqlite3_value_bytes             AnyOffice_sqlite3_value_bytes
#define  sqlite3_value_bytes16             AnyOffice_sqlite3_value_bytes16
#define  sqlite3_value_double             AnyOffice_sqlite3_value_double
#define  sqlite3_value_int             AnyOffice_sqlite3_value_int
#define  sqlite3_value_int64             AnyOffice_sqlite3_value_int64
#define  sqlite3_value_numeric_type             AnyOffice_sqlite3_value_numeric_type
#define  sqlite3_value_text             AnyOffice_sqlite3_value_text

#define  sqlite3_value_type             AnyOffice_sqlite3_value_type
#define  sqlite3_vfs_find             AnyOffice_sqlite3_vfs_find
#define  sqlite3_vfs_register             AnyOffice_sqlite3_vfs_register
#define  sqlite3_vfs_unregister             AnyOffice_sqlite3_vfs_unregister
#define  sqlite3_vmprintf             AnyOffice_sqlite3_vmprintf
#define  sqlite3_vsnprintf             AnyOffice_sqlite3_vsnprintf
#define  sqlite3_vtab_config             AnyOffice_sqlite3_vtab_config
#define  sqlite3_vtab_on_conflict             AnyOffice_sqlite3_vtab_on_conflict
#define  sqlite3_wal_autocheckpoint             AnyOffice_sqlite3_wal_autocheckpoint
#define  sqlite3_wal_checkpoint                 AnyOffice_sqlite3_wal_checkpoint
#define  sqlite3_wal_checkpoint_v2             AnyOffice_sqlite3_wal_checkpoint_v2
#define  sqlite3_wal_hook                       AnyOffice_sqlite3_wal_hook
#define  sqlite3_rekey_v2                       AnyOffice_sqlite3_rekey_v2
#define  sqlite3_rekey               AnyOffice_sqlite3_key

#ifndef SQLITE_OMIT_UTF16
#define  sqlite3_open16             AnyOffice_sqlite3_open16
#define  sqlite3_errmsg16             AnyOffice_sqlite3_errmsg16
#define  sqlite3_create_function16             AnyOffice_sqlite3_create_function16
#define  sqlite3_complete16             AnyOffice_sqlite3_complete16
#define  sqlite3_column_text16             AnyOffice_sqlite3_column_text16
#define  sqlite3_bind_text16             AnyOffice_sqlite3_bind_text16
#define  sqlite3_collation_needed16             AnyOffice_sqlite3_collation_needed16
#define  sqlite3_value_text16             AnyOffice_sqlite3_value_text16
#define  sqlite3_value_text16be             AnyOffice_sqlite3_value_text16be
#define  sqlite3_value_text16le             AnyOffice_sqlite3_value_text16le
#define  sqlite3_result_error16             AnyOffice_sqlite3_result_error16
#define  sqlite3_result_text16             AnyOffice_sqlite3_result_text16
#define  sqlite3_result_text16be             AnyOffice_sqlite3_result_text16be
#define  sqlite3_result_text16le             AnyOffice_sqlite3_result_text16le
#define  sqlite3_create_collation16             AnyOffice_sqlite3_create_collation16
#define  sqlite3_column_name16             AnyOffice_sqlite3_column_name16
#define  sqlite3_prepare16             AnyOffice_sqlite3_prepare16
#endif


#ifndef SQLITE_OMIT_DEPRECATED
#define  sqlite3_aggregate_count            AnyOffice_sqlite3_aggregate_count
#define  sqlite3_expired             AnyOffice_sqlite3_expired
#define  sqlite3_transfer_bindings             AnyOffice_sqlite3_transfer_bindings
#define  sqlite3_global_recover             AnyOffice_sqlite3_global_recover
#define  sqlite3_thread_cleanup             AnyOffice_sqlite3_thread_cleanup
#define  sqlite3_memory_alarm             AnyOffice_sqlite3_memory_alarm
#endif

#ifndef SQLITE_OMIT_DECLTYPE
#define  sqlite3_column_decltype             AnyOffice_sqlite3_column_decltype
#ifndef SQLITE_OMIT_UTF16
#define  sqlite3_column_decltype16             AnyOffice_sqlite3_column_decltype16
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif



