/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : tools_crypto.h
  版 本 号   : 初稿
  作    者   : y90004712
  生成日期   : 2014年11月20日
  最近修改   :
  功能描述   : openssl适配
  函数列表   :
*
*

  修改历史   :
  1.日    期   : 2014年11月20日
    作    者   : y90004712
    修改内容   : 创建文件

******************************************************************************/
#ifndef __TOOLS_CRYPTO__
#define __TOOLS_CRYPTO__


#include "tools_crypto_impl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define CRYPTO_LOCK		1
#define CRYPTO_UNLOCK		2
#define CRYPTO_READ		4
#define CRYPTO_WRITE		8

#define SSL_ERROR_NONE			0
#define SSL_ERROR_SSL			1
#define SSL_ERROR_WANT_READ		2
#define SSL_ERROR_WANT_WRITE		3
#define SSL_ERROR_WANT_X509_LOOKUP	4
#define SSL_ERROR_SYSCALL		5 /* look at error stack/return value/errno */
#define SSL_ERROR_ZERO_RETURN		6
#define SSL_ERROR_WANT_CONNECT		7
#define SSL_ERROR_WANT_ACCEPT		8

/* use either SSL_VERIFY_NONE or SSL_VERIFY_PEER, the last 2 options
 * are 'ored' with SSL_VERIFY_PEER if they are desired */
#define SSL_VERIFY_NONE			0x00
#define SSL_VERIFY_PEER			0x01
#define SSL_VERIFY_FAIL_IF_NO_PEER_CERT	0x02
#define SSL_VERIFY_CLIENT_ONCE		0x04


/* 外部需要操作EVP_CIPHER结构，打桩定义，与内部定义字段长度保持一致 */
#undef EVP_CIPHER
typedef struct anyoffice_evp_cipher_st
{
    int nid;
    int block_size;
    int key_len;		/* Default value for variable length ciphers */
    int iv_len;
    unsigned long flags;	/* Various flags */
    void *init;	/* init key func */
    void *do_cipher; /* encrypt/decrypt data func */
    void *cleanup; /* cleanup ctx func */
    int ctx_size;		/* how big ctx->cipher_data needs to be */
    void *set_asn1_parameters; /* Populate a ASN1_TYPE with parameters func */
    void *get_asn1_parameters; /* Get parameters from a ASN1_TYPE func */
    void *ctrl; /* Miscellaneous operations func */
    void *app_data;		/* Application data */
} EVP_CIPHER;



/*****************************************************************
                    openssl接口封装
*****************************************************************/
#define EVP_MD_CTX_create      AnyOffice_EVP_MD_CTX_create
#define EVP_md5                AnyOffice_EVP_md5
#define EVP_DigestInit         AnyOffice_EVP_DigestInit
#define EVP_DigestUpdate       AnyOffice_EVP_DigestUpdate
#define EVP_DigestFinal        AnyOffice_EVP_DigestFinal
#define EVP_MD_CTX_destroy     AnyOffice_EVP_MD_CTX_destroy

#define EVP_CipherInit         AnyOffice_EVP_CipherInit
#define EVP_Cipher             AnyOffice_EVP_Cipher
#define EVP_CIPHER_CTX_cleanup AnyOffice_EVP_CIPHER_CTX_cleanup
#define EVP_aes_128_cbc        AnyOffice_EVP_aes_128_cbc
#define EVP_aes_256_cbc        AnyOffice_EVP_aes_256_cbc

#define SSL_get_error          AnyOffice_SSL_get_error
#define ERR_clear_error        AnyOffice_ERR_clear_error

#define SSL_CTX_new            AnyOffice_SSL_CTX_new
#define SSL_free               AnyOffice_SSL_free
#define SSL_CTX_free           AnyOffice_SSL_CTX_free
#define SSL_CTX_set_verify     AnyOffice_SSL_CTX_set_verify
#define SSL_CTX_load_verify_locations     AnyOffice_SSL_CTX_load_verify_locations
#define d2i_AutoPrivateKey                AnyOffice_d2i_AutoPrivateKey
#define d2i_X509                          AnyOffice_d2i_X509
#define i2d_X509                          AnyOffice_i2d_X509
#define SSL_load_client_CA_file           AnyOffice_SSL_load_client_CA_file
#define SSL_CTX_set_client_CA_list        AnyOffice_SSL_CTX_set_client_CA_list
#define SSL_CTX_set_ex_data               AnyOffice_SSL_CTX_set_ex_data
#define SSL_CTX_set_client_cert_cb        AnyOffice_SSL_CTX_set_client_cert_cb
#define SSL_library_init                  AnyOffice_SSL_library_init
#define SSL_connect                       AnyOffice_SSL_connect
#define OpenSSL_add_all_digests           AnyOffice_OpenSSL_add_all_digests
#define OPENSSL_add_all_algorithms_noconf AnyOffice_OPENSSL_add_all_algorithms_noconf
#define OpenSSL_add_all_ciphers           AnyOffice_OpenSSL_add_all_ciphers
#define CRYPTO_num_locks                  AnyOffice_CRYPTO_num_locks
#define CRYPTO_malloc                     AnyOffice_CRYPTO_malloc
#define CRYPTO_set_id_callback            AnyOffice_CRYPTO_set_id_callback
#define CRYPTO_set_locking_callback       AnyOffice_CRYPTO_set_locking_callback
#define SSL_CTX_get_ex_data               AnyOffice_SSL_CTX_get_ex_data
#define TLSv1_client_method               AnyOffice_TLSv1_client_method
#define SSLv23_client_method              AnyOffice_SSLv23_client_method
#define SSL_get_peer_certificate          AnyOffice_SSL_get_peer_certificate
#define SSL_new                           AnyOffice_SSL_new
#define SSL_write                         AnyOffice_SSL_write
#define SSL_read                          AnyOffice_SSL_read
#define SSL_CTX_set_app_data(ctx,arg)	 (SSL_CTX_set_ex_data(ctx,0,(char *)arg))
#define SSL_CTX_get_app_data(ctx)	(SSL_CTX_get_ex_data(ctx,0))
#define OPENSSL_malloc(num)	CRYPTO_malloc((int)num,__FILE__,__LINE__)
#ifdef OPENSSL_LOAD_CONF
#define OpenSSL_add_all_algorithms() \
		OPENSSL_add_all_algorithms_conf()
#else
#define OpenSSL_add_all_algorithms() \
		OPENSSL_add_all_algorithms_noconf()
#endif


#define SHA1                   AnyOffice_SHA1


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

