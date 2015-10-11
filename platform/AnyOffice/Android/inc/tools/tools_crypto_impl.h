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
#ifndef __TOOLS_CRYPTO_IMPL__
#define __TOOLS_CRYPTO_IMPL__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/*****************************************************************
                    openssl结构桩定义
*****************************************************************/
#ifndef HEADER_OPENSSL_TYPES_H
#ifndef EVP_MD_CTX
#define EVP_MD_CTX void
#endif

#ifndef EVP_MD
#define EVP_MD void
#endif

#ifndef EVP_CIPHER_CTX
#define EVP_CIPHER_CTX void
#endif

#ifndef EVP_CIPHER
#define EVP_CIPHER void
#endif

#ifndef SSL
#define SSL void
#endif

#ifndef X509
#define X509 void
#endif

#ifndef SSL_CTX
#define SSL_CTX void
#endif 

#ifndef SSL_METHOD
#define SSL_METHOD  void
#endif

#ifndef EVP_PKEY
#define EVP_PKEY void
#endif

#ifndef X509_STORE_CTX
#define X509_STORE_CTX void
#endif

#ifndef STACK_OF
#define STACK_OF(X509_NAME) void
#endif
#endif

/*****************************************************************
                    openssl接口封装
*****************************************************************/

EVP_MD_CTX *AnyOffice_EVP_MD_CTX_create(void);
const EVP_MD *AnyOffice_EVP_md5(void);
int AnyOffice_EVP_DigestInit(EVP_MD_CTX *ctx, const EVP_MD *type);
int AnyOffice_EVP_DigestUpdate(EVP_MD_CTX *ctx, const void *data, int count);
int AnyOffice_EVP_DigestFinal(EVP_MD_CTX *ctx, unsigned char *md, unsigned int *size);
void AnyOffice_EVP_MD_CTX_destroy(EVP_MD_CTX *ctx);
EVP_CIPHER_CTX *AnyOffice_EVP_CipherInit(const EVP_CIPHER *cipher,
                                             const unsigned char *key, const unsigned char *iv, int enc);
int AnyOffice_EVP_Cipher(EVP_CIPHER_CTX *ctx, unsigned char *out, const unsigned char *in, unsigned int inl);
int AnyOffice_EVP_CIPHER_CTX_cleanup(EVP_CIPHER_CTX *c);
const EVP_CIPHER *AnyOffice_EVP_aes_128_cbc(void);
const EVP_CIPHER *AnyOffice_EVP_aes_256_cbc(void);

int AnyOffice_SSL_get_error(const SSL *s,int i);
void AnyOffice_ERR_clear_error(void);

SSL_CTX *AnyOffice_SSL_CTX_new(SSL_METHOD *meth);
void AnyOffice_SSL_free(SSL *s);
void AnyOffice_SSL_CTX_free(SSL_CTX *a);
void AnyOffice_SSL_CTX_set_verify(SSL_CTX *ctx,int mode,int (*cb)(int, X509_STORE_CTX *));
int AnyOffice_SSL_CTX_load_verify_locations(SSL_CTX *ctx, const char *CAfile, const char *CApath);
EVP_PKEY* AnyOffice_d2i_AutoPrivateKey(EVP_PKEY **a, const unsigned char **pp, long length);
X509* AnyOffice_d2i_X509(X509 **a, const unsigned char **pp, long length);
X509* AnyOffice_i2d_X509(X509 *a, unsigned char **pp);
STACK_OF(X509_NAME)* AnyOffice_SSL_load_client_CA_file(const char *file);
void AnyOffice_SSL_CTX_set_client_CA_list(SSL_CTX *ctx, STACK_OF(X509_NAME) *name_list);
int AnyOffice_SSL_CTX_set_ex_data(SSL_CTX *s,int idx,void *arg);
void AnyOffice_SSL_CTX_set_client_cert_cb(SSL_CTX *ctx,	int (*cb)(SSL *ssl, X509 **x509, EVP_PKEY **pkey));
int AnyOffice_SSL_library_init();
int AnyOffice_SSL_connect(SSL *s);
void AnyOffice_OpenSSL_add_all_digests();
void AnyOffice_OPENSSL_add_all_algorithms_noconf(void);
void AnyOffice_OpenSSL_add_all_ciphers(void);
int AnyOffice_CRYPTO_num_locks(void);
void *AnyOffice_CRYPTO_malloc(int num, const char *file, int line);
void AnyOffice_CRYPTO_set_id_callback(unsigned long (*func)(void));
void AnyOffice_CRYPTO_set_locking_callback(void (*func)(int mode,int type, const char *file,int line));
void *AnyOffice_SSL_CTX_get_ex_data(const SSL *s,int idx);
SSL_METHOD *AnyOffice_TLSv1_client_method();
SSL_METHOD *AnyOffice_SSLv23_client_method();
X509 *AnyOffice_SSL_get_peer_certificate(const SSL *s);
SSL *AnyOffice_SSL_new(SSL_CTX *ctx);
int AnyOffice_SSL_write(SSL *s,const void *buf,int num);
int AnyOffice_SSL_read(SSL *s,void *buf,int num);
void AnyOffice_OPENSSL_add_all_algorithms_conf();

unsigned char *AnyOffice_SHA1(const unsigned char *d, unsigned int  n, unsigned char *md);
unsigned char *AnyOffice_SHA256(const unsigned char *d, unsigned int  n, unsigned char *md);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
