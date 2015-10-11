#ifndef SVN_HTTP_EX_H
#define	SVN_HTTP_EX_H

#include <sys/select.h>
#include <sys/socket.h>  // TODO:后续可替代成AnyOffice socket定义


#ifdef	__cplusplus
extern "C" {

#endif

#ifndef  SvnHttp
#define SvnHttp void
#endif

#define SVN_HTTP_ERR -1 
#define SVN_HTTP_OK 0

#ifndef WIN32
#define CURL_EXTERN
#endif

/* 设置链表 */
struct http_slist {
	char *data;
	struct http_slist *next;
};

/*	svn_http_setopt 设置opt:SVN_HTTPOPT_POSTREDIR 取值 */
#define SVNHTTP_REDIR_GET_ALL  0
#define SVNHTTP_REDIR_POST_301 1
#define SVNHTTP_REDIR_POST_302 2
#define SVNHTTP_REDIR_POST_ALL (SVNHTTP_REDIR_POST_301|SVNHTTP_REDIR_POST_302)

/*	svn_http_setopt 设置opt:SVN_HTTPOPT_TIMECONDITION 取值 */
typedef enum {
	SVNHTTP_TIMECOND_NONE,
	SVNHTTP_TIMECOND_IFMODSINCE,
	SVNHTTP_TIMECOND_IFUNMODSINCE,
	SVNHTTP_TIMECOND_LASTMOD,
	SVNHTTP_TIMECOND_LAST
} svn_TimeCond;


/* These enums are for use with the SVN_HTTPOPT_HTTP_VERSION option. */
enum {
	SVN_HTTP_VERSION_NONE, /* setting this means we don't care, and that we'd
							like the library to choose the best possible
							for us! */
	SVN_HTTP_VERSION_1_0,  /* please use HTTP 1.0 in the request */
	SVN_HTTP_VERSION_1_1,  /* please use HTTP 1.1 in the request */

	SVN_HTTP_VERSION_LAST /* *ILLEGAL* http version */
};

/* 验证方式	Authentication  */
#define SVNHTTPAUTH_NONE         0       /* nothing */
#define SVNHTTPAUTH_BASIC        (1<<0)  /* Basic (default) */
#define SVNHTTPAUTH_DIGEST       (1<<1)  /* Digest */
#define SVNHTTPAUTH_GSSNEGOTIATE (1<<2)  /* GSS-Negotiate */
#define SVNHTTPAUTH_NTLM         (1<<3)  /* NTLM */
#define SVNHTTPAUTH_DIGEST_IE    (1<<4)  /* Digest with IE flavour */
#define SVNHTTPAUTH_ONLY         (1<<31) /* used together with a single other
										 type to force no auth or just that
										 single type */
#define SVNHTTPAUTH_ANY (~SVNHTTPAUTH_DIGEST_IE)  /* all fine types set */
#define SVNHTTPAUTH_ANYSAFE (~(SVNHTTPAUTH_BASIC|SVNHTTPAUTH_DIGEST_IE))

/*	代理类型  */
typedef enum {
	SVNHTTPPROXY_HTTP = 0,		
	SVNHTTPPROXY_HTTP_1_0 = 1, 
	SVNHTTPPROXY_SOCKS4 = 4,	
	SVNHTTPPROXY_SOCKS5 = 5,	
	SVNHTTPPROXY_SOCKS4A = 6,	
	SVNHTTPPROXY_SOCKS5_HOSTNAME = 7 /* Use the SOCKS5 protocol but pass along the
									    host name rather than the IP address */
} svnhttp_proxytype; 

/* Below here follows defines for the SVN_HTTPOPT_IPRESOLVE option. If a host
   name resolves addresses using more than one IP protocol version, this
   option might be handy to force libcurl to use a specific IP version. */
#define SVNHTTP_IPRESOLVE_WHATEVER 0 /* default, resolves addresses to all IP
										versions that your system allows */
#define SVNHTTP_IPRESOLVE_V4       1 /* resolve to ipv4 addresses */
#define SVNHTTP_IPRESOLVE_V6       2 /* resolve to ipv6 addresses */


/* parameter for the SVN_HTTPOPT_USE_SSL option */
typedef enum {
	SVNHTTPUSESSL_NONE,    /* do not attempt to use SSL */
	SVNHTTPUSESSL_TRY,     /* try using SSL, proceed anyway otherwise */
	SVNHTTPUSESSL_CONTROL, /* SSL for the control connection or fail */
	SVNHTTPUSESSL_ALL,     /* SSL for all communication or fail */
	SVNHTTPUSESSL_LAST     /* not an option, never use */
} svnhttp_usessl;

/*	设置 SVN_HTTPOPT_HTTPPOST 选择时 参数 */
struct svn_httppost {
	struct svn_httppost *next;       /* next entry in the list */
	char *name;                       /* pointer to allocated name */
	long namelength;                  /* length of name length */
	char *contents;                   /* pointer to allocated data contents */
	long contentslength;              /* length of contents field */
	char *buffer;                     /* pointer to allocated buffer contents */
	long bufferlength;                /* length of buffer field */
	char *contenttype;                /* Content-Type */
	struct http_slist* contentheader; /* list of extra headers for this form */
	struct svn_httppost *more;       /* if one field name has more than one
									  file, this link should link to following
									  files */
	long flags;                       /* as defined below */
#define HTTPPOST_FILENAME (1<<0)    /* specified content is a file name */
#define HTTPPOST_READFILE (1<<1)    /* specified content is a file name */
#define HTTPPOST_PTRNAME (1<<2)     /* name is only stored pointer do not free in formfree */
#define HTTPPOST_PTRCONTENTS (1<<3) /* contents is only stored pointer do not free in formfree */
#define HTTPPOST_BUFFER (1<<4)      /* upload file from buffer */
#define HTTPPOST_PTRBUFFER (1<<5)   /* upload file from pointer contents */
#define HTTPPOST_CALLBACK (1<<6)    /* upload file contents by using the
									  regular read callback to get the data
									  and pass the given pointer as custom
									  pointer */

	char *showfilename;               /* The file name to show. If not set, the
									  actual file name will be used (if this
									  is a file part) */
	void *userp;                      /* custom pointer used for
									  HTTPPOST_CALLBACK posts */
};

/* SVNPROTO_ defines are for the SVN_HTTPOPT_PROTOCOLS options */
#define SVNPROTO_HTTP   (1<<0)
#define SVNPROTO_HTTPS  (1<<1)
#define SVNPROTO_FTP    (1<<2)
#define SVNPROTO_FTPS   (1<<3)
#define SVNPROTO_SCP    (1<<4)
#define SVNPROTO_SFTP   (1<<5)
#define SVNPROTO_TELNET (1<<6)
#define SVNPROTO_LDAP   (1<<7)
#define SVNPROTO_LDAPS  (1<<8)
#define SVNPROTO_DICT   (1<<9)
#define SVNPROTO_FILE   (1<<10)
#define SVNPROTO_TFTP   (1<<11)
#define SVNPROTO_IMAP   (1<<12)
#define SVNPROTO_IMAPS  (1<<13)
#define SVNPROTO_POP3   (1<<14)
#define SVNPROTO_POP3S  (1<<15)
#define SVNPROTO_SMTP   (1<<16)
#define SVNPROTO_SMTPS  (1<<17)
#define SVNPROTO_RTSP   (1<<18)
#define SVNPROTO_RTMP   (1<<19)
#define SVNPROTO_RTMPT  (1<<20)
#define SVNPROTO_RTMPE  (1<<21)
#define SVNPROTO_RTMPTE (1<<22)
#define SVNPROTO_RTMPS  (1<<23)
#define SVNPROTO_RTMPTS (1<<24)
#define SVNPROTO_GOPHER (1<<25)
#define SVNPROTO_ALL    (~0) /* enable everything */

/*svn_http_setopt 设置选项*/

#define SVNHTTPTYPE_LONG          0
#define SVNHTTPTYPE_OBJECTPOINT   10000
#define SVNHTTPTYPE_FUNCTIONPOINT 20000
#define SVNHTTPTYPE_OFF_T         30000
#define	SVNHTTPTYPE_POINT		  40000

typedef enum 
{
	SVN_HTTPOPT_DNS_CACHE_TIMEOUT = SVNHTTPTYPE_LONG + 92,

	/* remember we want this enabled */
	SVN_HTTPOPT_DNS_USE_GLOBAL_CACHE = SVNHTTPTYPE_LONG + 91,

	 /* set a list of cipher we want to use in the SSL connection */
	SVN_HTTPOPT_SSL_CIPHER_LIST = SVNHTTPTYPE_OBJECTPOINT + 83,

	/*
	* This is the path name to a file that contains random data to seed
	* the random SSL stuff with. The file is only used for reading.
	*/
	SVN_HTTPOPT_RANDOM_FILE =SVNHTTPTYPE_OBJECTPOINT + 76,

	/*
	* The Entropy Gathering Daemon socket pathname
	*/
	SVN_HTTPOPT_EGDSOCKET =SVNHTTPTYPE_OBJECTPOINT + 77,

	/*
	* Set the absolute number of maximum simultaneous alive connection that
	* libcurl is allowed to have.
	*/
	SVN_HTTPOPT_MAXCONNECTS =SVNHTTPTYPE_LONG + 71,

	/*
	* When this transfer is done, it must not be left to be reused by a
	* subsequent transfer but shall be closed immediately.
	*/
	SVN_HTTPOPT_FORBID_REUSE =SVNHTTPTYPE_LONG + 75	,

	/*
	* This transfer shall not use a previously cached connection but
	* should be made with a fresh new connect!
	*/
	SVN_HTTPOPT_FRESH_CONNECT = SVNHTTPTYPE_LONG + 74,

	/*
	* Verbose means infof() calls that give a lot of information about
	* the connection and transfer procedures as well as internal choices.
	*/
	SVN_HTTPOPT_VERBOSE = SVNHTTPTYPE_LONG + 41,

	/*
	* Set to include the header in the general data output stream.
	*/
	SVN_HTTPOPT_HEADER = SVNHTTPTYPE_LONG + 42,

	/*
	* Shut off the internal supported progress meter
	*/
	SVN_HTTPOPT_NOPROGRESS = SVNHTTPTYPE_LONG + 43,

	/*
	* Do not include the body part in the output data stream.
	*/
	SVN_HTTPOPT_NOBODY = SVNHTTPTYPE_LONG + 44,

	/*
	* Don't output the >=300 error code HTML-page, but instead only
	* return error.
	*/
	SVN_HTTPOPT_FAILONERROR = SVNHTTPTYPE_LONG + 45,

	/*
	* We want to sent data to the remote host. If this is HTTP, that equals
	* using the PUT request.
	*/
	SVN_HTTPOPT_UPLOAD = SVNHTTPTYPE_LONG + 46,
	SVN_HTTPOPT_PUT = SVNHTTPTYPE_LONG + 54,

	/*
	* Try to get the file time of the remote document. The time will
	* later (possibly) become available using curl_easy_getinfo().
	*/
	SVN_HTTPOPT_FILETIME = SVNHTTPTYPE_LONG + 69,

	/*
	* An FTP option that modifies an upload to create missing directories on
	* the server.
	*/
	SVN_HTTPOPT_FTP_CREATE_MISSING_DIRS = SVNHTTPTYPE_LONG + 110,

	/*
	* Option that specifies how quickly an server response must be obtained
	* before it is considered failure. For pingpong protocols.
	*/
	SVN_HTTPOPT_SERVER_RESPONSE_TIMEOUT = SVNHTTPTYPE_LONG + 112,

	/*
	* TFTP option that specifies the block size to use for data transmission
	*/
	SVN_HTTPOPT_TFTP_BLKSIZE = SVNHTTPTYPE_LONG + 178,

	/*
	* An option that changes the command to one that asks for a list
	* only, no file info details.
	*/
	SVN_HTTPOPT_DIRLISTONLY = SVNHTTPTYPE_LONG + 48,

	/*
	* We want to upload and append to an existing file.
	*/
	SVN_HTTPOPT_APPEND = SVNHTTPTYPE_LONG + 50,

	/*
	* How do access files over FTP.
	*/
	SVN_HTTPOPT_FTP_FILEMETHOD = SVNHTTPTYPE_LONG + 138,

	/*
	* Parse the $HOME/.netrc file
	*/
	SVN_HTTPOPT_NETRC = SVNHTTPTYPE_LONG + 51,

	/*
	* Use this file instead of the $HOME/.netrc file
	*/
	SVN_HTTPOPT_NETRC_FILE =SVNHTTPTYPE_OBJECTPOINT + 118,

	/*
	* This option was previously named 'FTPASCII'. Renamed to work with
	* more protocols than merely FTP.
	*
	* Transfer using ASCII (instead of BINARY).
	*/
	SVN_HTTPOPT_TRANSFERTEXT = SVNHTTPTYPE_LONG + 53,
	
	/*
	* Set HTTP time condition. This must be one of the defines in the
	* curl/curl.h header file.
	*/
	SVN_HTTPOPT_TIMECONDITION = SVNHTTPTYPE_LONG + 33,

	/*
	* This is the value to compare with the remote document with the
	* method set with SVN_HTTPOPT_TIMECONDITION
	*/
	SVN_HTTPOPT_TIMEVALUE = SVNHTTPTYPE_LONG + 34,

	/*
	* Set explicit SSL version to try to connect with, as some SSL
	* implementations are lame.
	*/
	SVN_HTTPOPT_SSLVERSION = SVNHTTPTYPE_LONG + 32,

	/*
	* Switch on automatic referer that gets set if curl follows locations.
	*/
	SVN_HTTPOPT_AUTOREFERER = SVNHTTPTYPE_LONG + 58,

	/*
	* String to use at the value of Accept-Encoding header.
	*
	* If the encoding is set to "" we use an Accept-Encoding header that
	* encompasses all the encodings we support.
	* If the encoding is set to NULL we don't send an Accept-Encoding header
	* and ignore an received Content-Encoding header.
	*
	*/
	SVN_HTTPOPT_ACCEPT_ENCODING = SVNHTTPTYPE_OBJECTPOINT + 102,


	SVN_HTTPOPT_TRANSFER_ENCODING = SVNHTTPTYPE_LONG + 207,

	/*
	* Follow Location: header hints on a HTTP-server.
	*/
	SVN_HTTPOPT_FOLLOWLOCATION = SVNHTTPTYPE_LONG + 52,

	/*
	* Send authentication (user+password) when following locations, even when
	* hostname changed.
	*/
	SVN_HTTPOPT_UNRESTRICTED_AUTH = SVNHTTPTYPE_LONG + 105,

	/*
	* The maximum amount of hops you allow curl to follow Location:
	* headers. This should mostly be used to detect never-ending loops.
	*/
	SVN_HTTPOPT_MAXREDIRS = SVNHTTPTYPE_LONG + 68,

	/*
	* Set the behaviour of POST when redirecting
	* CURL_REDIR_GET_ALL - POST is changed to GET after 301 and 302
	* CURL_REDIR_POST_301 - POST is kept as POST after 301
	* CURL_REDIR_POST_302 - POST is kept as POST after 302
	* CURL_REDIR_POST_ALL - POST is kept as POST after 301 and 302
	* other - POST is kept as POST after 301 and 302
	*/
	SVN_HTTPOPT_POSTREDIR = SVNHTTPTYPE_LONG + 161,

	/* Does this option serve a purpose anymore? Yes it does, when
	SVN_HTTPOPT_POSTFIELDS isn't used and the POST data is read off the
	callback! */
	SVN_HTTPOPT_POST = SVNHTTPTYPE_LONG + 47,
	
	/*
	* A string with POST data. Makes curl HTTP POST. Even if it is NULL.
	* If needed, SVN_HTTPOPT_POSTFIELDSIZE must have been set prior to
	*  SVN_HTTPOPT_COPYPOSTFIELDS and not altered later.
	*/
	SVN_HTTPOPT_COPYPOSTFIELDS = SVNHTTPTYPE_OBJECTPOINT + 165,

	/*
	* Like above, but use static data instead of copying it.
	*/
	SVN_HTTPOPT_POSTFIELDS = SVNHTTPTYPE_OBJECTPOINT + 15,

	/*
	* The size of the POSTFIELD data to prevent libcurl to do strlen() to
	* figure it out. Enables binary posts.
	*/
	SVN_HTTPOPT_POSTFIELDSIZE = SVNHTTPTYPE_LONG + 60,
	
	/*
	* The size of the POSTFIELD data to prevent libcurl to do strlen() to
	* figure it out. Enables binary posts.
	*/
	SVN_HTTPOPT_POSTFIELDSIZE_LARGE = SVNHTTPTYPE_OFF_T + 120,


	/*
	* Set to make us do HTTP POST
	*/
	SVN_HTTPOPT_HTTPPOST = SVNHTTPTYPE_OBJECTPOINT + 24,

	/*
	* String to set in the HTTP Referer: field.
	*/
	SVN_HTTPOPT_REFERER = SVNHTTPTYPE_OBJECTPOINT + 16,

	/*
	* String to use in the HTTP User-Agent field
	*/
	SVN_HTTPOPT_USERAGENT = SVNHTTPTYPE_OBJECTPOINT + 18,

	/*
	* Set a list with HTTP headers to use (or replace internals with)
	*/
	SVN_HTTPOPT_HTTPHEADER = SVNHTTPTYPE_OBJECTPOINT + 23,

	/*
	* Set a list of aliases for HTTP 200 in response header
	*/
	SVN_HTTPOPT_HTTP200ALIASES = SVNHTTPTYPE_OBJECTPOINT + 104,

	/*
	* Cookie string to send to the remote server in the request.
	*/
	SVN_HTTPOPT_COOKIE = SVNHTTPTYPE_OBJECTPOINT + 22,

	/*
	* Set cookie file to read and parse. Can be used multiple times.
	*/
	SVN_HTTPOPT_COOKIEFILE = SVNHTTPTYPE_OBJECTPOINT + 31,

	/*
	* Set cookie file name to dump all cookies to when we're done.
	*/
	SVN_HTTPOPT_COOKIEJAR = SVNHTTPTYPE_OBJECTPOINT + 82,

	/*
	* Set this option to TRUE to start a new "cookie session". It will
	* prevent the forthcoming read-cookies-from-file actions to accept
	* cookies that are marked as being session cookies, as they belong to a
	* previous session.
	*
	* In the original Netscape cookie spec, "session cookies" are cookies
	* with no expire date set. RFC2109 describes the same action if no
	* 'Max-Age' is set and RFC2965 includes the RFC2109 description and adds
	* a 'Discard' action that can enforce the discard even for cookies that
	* have a Max-Age.
	*
	* We run mostly with the original cookie spec, as hardly anyone implements
	* anything else.
	*/
	SVN_HTTPOPT_COOKIESESSION = SVNHTTPTYPE_LONG + 96,

	SVN_HTTPOPT_COOKIELIST = SVNHTTPTYPE_OBJECTPOINT + 135,

	/*
	* Set to force us do HTTP GET
	*/
	SVN_HTTPOPT_HTTPGET = SVNHTTPTYPE_LONG + 80,

	/*
	* This sets a requested HTTP version to be used. The value is one of
	* the listed enums in curl/curl.h.
	*/
	SVN_HTTPOPT_HTTP_VERSION = SVNHTTPTYPE_LONG + 84,

	/*
	* Set HTTP Authentication type BITMASK.
	*/
	SVN_HTTPOPT_HTTPAUTH = SVNHTTPTYPE_LONG + 107,

	/*
	* Set a custom string to use as request
	*/
	SVN_HTTPOPT_CUSTOMREQUEST = SVNHTTPTYPE_OBJECTPOINT + 36,

	/*
	* Tunnel operations through the proxy instead of normal proxy use
	*/
	SVN_HTTPOPT_HTTPPROXYTUNNEL = SVNHTTPTYPE_LONG + 61,

	/*
	* Explicitly set HTTP proxy port number.
	*/
	SVN_HTTPOPT_PROXYPORT = SVNHTTPTYPE_LONG + 59,

	/*
	* Set HTTP Authentication type BITMASK.
	*/
	SVN_HTTPOPT_PROXYAUTH = SVNHTTPTYPE_LONG + 111,

	/*
	* Set proxy server:port to use as HTTP proxy.
	*
	* If the proxy is set to "" we explicitly say that we don't want to use a
	* proxy (even though there might be environment variables saying so).
	*
	* Setting it to NULL, means no proxy but allows the environment variables
	* to decide for us.
	*/
	SVN_HTTPOPT_PROXY = SVNHTTPTYPE_OBJECTPOINT + 4,

	/*
	* Set proxy type. HTTP/HTTP_1_0/SOCKS4/SOCKS4a/SOCKS5/SOCKS5_HOSTNAME
	*/
	SVN_HTTPOPT_PROXYTYPE = SVNHTTPTYPE_LONG + 101,

	/*
	* set transfer mode (;type=<a|i>) when doing FTP via an HTTP proxy
	*/
	SVN_HTTPOPT_PROXY_TRANSFER_MODE = SVNHTTPTYPE_LONG + 166,

	/*
	* Set gssapi service name
	*/
	SVN_HTTPOPT_SOCKS5_GSSAPI_SERVICE = SVNHTTPTYPE_OBJECTPOINT + 179,

	/*
	* set flag for nec socks5 support
	*/
	SVN_HTTPOPT_SOCKS5_GSSAPI_NEC = SVNHTTPTYPE_LONG + 180,

	/*
	* Custom pointer to pass the header write callback function
	*/
	SVN_HTTPOPT_WRITEHEADER = SVNHTTPTYPE_OBJECTPOINT + 29,

	/*
	* Error buffer provided by the caller to get the human readable
	* error string in.
	*/
	SVN_HTTPOPT_ERRORBUFFER = SVNHTTPTYPE_OBJECTPOINT + 10,

	/*
	* FILE pointer to write to or include in the data write callback
	*/
	SVN_HTTPOPT_FILE = SVNHTTPTYPE_OBJECTPOINT + 1,

	/*
	* Use FTP PORT, this also specifies which IP address to use
	*/
	SVN_HTTPOPT_FTPPORT = SVNHTTPTYPE_OBJECTPOINT + 17,


	SVN_HTTPOPT_FTP_USE_EPRT = SVNHTTPTYPE_LONG + 106,

	SVN_HTTPOPT_FTP_USE_EPSV = SVNHTTPTYPE_LONG + 85,

	SVN_HTTPOPT_FTP_USE_PRET = SVNHTTPTYPE_LONG + 188,

	SVN_HTTPOPT_FTP_SSL_CCC = SVNHTTPTYPE_LONG + 154,

	/*
	* Enable or disable FTP_SKIP_PASV_IP, which will disable/enable the
	* bypass of the IP address in PASV responses.
	*/
	SVN_HTTPOPT_FTP_SKIP_PASV_IP = SVNHTTPTYPE_LONG + 137,

	/*
	* FILE pointer to read the file to be uploaded from. Or possibly
	* used as argument to the read callback.
	*/
	SVN_HTTPOPT_INFILE = SVNHTTPTYPE_OBJECTPOINT + 9,
	
	/*
	* If known, this should inform curl about the file size of the
	* to-be-uploaded file.
	*/
	SVN_HTTPOPT_INFILESIZE = SVNHTTPTYPE_LONG + 14,

	/*
	* If known, this should inform curl about the file size of the
	* to-be-uploaded file.
	*/
	SVN_HTTPOPT_INFILESIZE_LARGE = SVNHTTPTYPE_OFF_T + 115,
	
	/*
	* The low speed limit that if transfers are below this for
	* SVN_HTTPOPT_LOW_SPEED_TIME, the transfer is aborted.
	*/
	SVN_HTTPOPT_LOW_SPEED_LIMIT = SVNHTTPTYPE_LONG + 19,

	/*
	* When transfer uploads are faster then SVN_HTTPOPT_MAX_SEND_SPEED_LARGE
	* bytes per second the transfer is throttled..
	*/
	SVN_HTTPOPT_MAX_SEND_SPEED_LARGE = SVNHTTPTYPE_OFF_T + 145,

	/*
	* When receiving data faster than SVN_HTTPOPT_MAX_RECV_SPEED_LARGE bytes per
	* second the transfer is throttled..
	*/
	SVN_HTTPOPT_MAX_RECV_SPEED_LARGE = SVNHTTPTYPE_OFF_T + 146,

	/*
	* The low speed time that if transfers are below the set
	* SVN_HTTPOPT_LOW_SPEED_LIMIT during this time, the transfer is aborted.
	*/
	SVN_HTTPOPT_LOW_SPEED_TIME = SVNHTTPTYPE_LONG + 20,

	/*
	* The URL to fetch.
	*/
	SVN_HTTPOPT_URL = SVNHTTPTYPE_OBJECTPOINT + 2,

	/*
	* The port number to use when getting the URL
	*/
	SVN_HTTPOPT_PORT = SVNHTTPTYPE_LONG + 3,

	/*
	* The maximum time you allow curl to use for a single transfer
	* operation.
	*/
	SVN_HTTPOPT_TIMEOUT = SVNHTTPTYPE_LONG + 13,
	

	SVN_HTTPOPT_TIMEOUT_MS = SVNHTTPTYPE_LONG + 155,

	/*
	* The maximum time you allow curl to use to connect.
	*/
	SVN_HTTPOPT_CONNECTTIMEOUT = SVNHTTPTYPE_LONG + 78,

	SVN_HTTPOPT_CONNECTTIMEOUT_MS = SVNHTTPTYPE_LONG + 156,

	/*
	* user:password to use in the operation
	*/
	SVN_HTTPOPT_USERPWD = SVNHTTPTYPE_OBJECTPOINT + 5,

	/*
	* authentication user name to use in the operation
	*/
	SVN_HTTPOPT_USERNAME = SVNHTTPTYPE_OBJECTPOINT + 173,

	/*
	* authentication password to use in the operation
	*/
	SVN_HTTPOPT_PASSWORD = SVNHTTPTYPE_OBJECTPOINT + 174,

	/*
	* List of RAW FTP commands to use after a transfer
	*/
	SVN_HTTPOPT_POSTQUOTE = SVNHTTPTYPE_OBJECTPOINT + 39,

	/*
	* List of RAW FTP commands to use prior to RETR (Wesley Laxton)
	*/
	SVN_HTTPOPT_PREQUOTE = SVNHTTPTYPE_OBJECTPOINT + 93,

	/*
	* List of RAW FTP commands to use before a transfer
	*/
	SVN_HTTPOPT_QUOTE = SVNHTTPTYPE_OBJECTPOINT + 28,

	/*
	* List of NAME:[address] names to populate the DNS cache with
	* Prefix the NAME with dash (-) to _remove_ the name from the cache.
	*
	* Names added with this API will remain in the cache until explicitly
	* removed or the handle is cleaned up.
	*
	* This API can remove any name from the DNS cache, but only entries
	* that aren't actually in use right now will be pruned immediately.
	*/
	SVN_HTTPOPT_RESOLVE = SVNHTTPTYPE_OBJECTPOINT + 203,

	/*
	* Progress callback function
	*/
	SVN_HTTPOPT_PROGRESSFUNCTION = SVNHTTPTYPE_FUNCTIONPOINT + 56,

	/*
	* Custom client data to pass to the progress callback
	*/
	SVN_HTTPOPT_PROGRESSDATA = SVNHTTPTYPE_OBJECTPOINT + 57,

	/*
	* user:password needed to use the proxy
	*/
	SVN_HTTPOPT_PROXYUSERPWD = SVNHTTPTYPE_OBJECTPOINT + 6,

	/*
	* authentication user name to use in the operation
	*/
	SVN_HTTPOPT_PROXYUSERNAME = SVNHTTPTYPE_OBJECTPOINT + 175,

	/*
	* authentication password to use in the operation
	*/
	SVN_HTTPOPT_PROXYPASSWORD = SVNHTTPTYPE_OBJECTPOINT + 176,

	/*
	* proxy exception list
	*/
	SVN_HTTPOPT_NOPROXY= SVNHTTPTYPE_OBJECTPOINT + 177,

	/*
	* What range of the file you want to transfer
	*/
	SVN_HTTPOPT_RANGE = SVNHTTPTYPE_OBJECTPOINT + 7,

	/*
	* Resume transfer at the give file position
	*/
	SVN_HTTPOPT_RESUME_FROM = SVNHTTPTYPE_LONG + 21,

	/*
	* Resume transfer at the give file position
	*/
	SVN_HTTPOPT_RESUME_FROM_LARGE = SVNHTTPTYPE_OFF_T + 116,

	/*
	* stderr write callback.
	*/
	SVN_HTTPOPT_DEBUGFUNCTION = SVNHTTPTYPE_FUNCTIONPOINT + 94,

	/*
	* Set to a void * that should receive all error writes. This
	* defaults to SVN_HTTPOPT_STDERR for normal operations.
	*/
	SVN_HTTPOPT_DEBUGDATA = SVNHTTPTYPE_OBJECTPOINT + 95,

	/*
	* Set to a FILE * that should receive all error writes. This
	* defaults to stderr for normal operations.
	*/
	SVN_HTTPOPT_STDERR = SVNHTTPTYPE_OBJECTPOINT + 37,

	/*
	* Set header write callback
	*/
	SVN_HTTPOPT_HEADERFUNCTION = SVNHTTPTYPE_FUNCTIONPOINT + 79,

	/*
	* Set data write callback
	*/
	SVN_HTTPOPT_WRITEFUNCTION = SVNHTTPTYPE_FUNCTIONPOINT + 11,

	/*
	* Read data callback
	*/
	SVN_HTTPOPT_READFUNCTION = SVNHTTPTYPE_FUNCTIONPOINT + 12,
	
	/*
	* Seek callback. Might be NULL.
	*/
	SVN_HTTPOPT_SEEKFUNCTION = SVNHTTPTYPE_FUNCTIONPOINT + 167,

	/*
	* Seek control callback. Might be NULL.
	*/
	SVN_HTTPOPT_SEEKDATA = SVNHTTPTYPE_OBJECTPOINT + 168,

	/*
	* "Convert from network encoding" callback
	*/
	SVN_HTTPOPT_CONV_FROM_NETWORK_FUNCTION		= SVNHTTPTYPE_FUNCTIONPOINT + 142,

	/*
	* "Convert to network encoding" callback
	*/
	SVN_HTTPOPT_CONV_TO_NETWORK_FUNCTION		= SVNHTTPTYPE_FUNCTIONPOINT + 143,

	/*
	* "Convert from UTF-8 encoding" callback
	*/
	SVN_HTTPOPT_CONV_FROM_UTF8_FUNCTION			= SVNHTTPTYPE_FUNCTIONPOINT + 144,

	/*
	* I/O control callback. Might be NULL.
	*/
	SVN_HTTPOPT_IOCTLFUNCTION = SVNHTTPTYPE_FUNCTIONPOINT + 130,

	/*
	* I/O control data pointer. Might be NULL.
	*/
	SVN_HTTPOPT_IOCTLDATA = SVNHTTPTYPE_OBJECTPOINT + 131,

	/*
	* String that holds file name of the SSL certificate to use
	*/
	SVN_HTTPOPT_SSLCERT = SVNHTTPTYPE_OBJECTPOINT + 25,

	/*
	* String that holds file type of the SSL certificate to use
	*/
	SVN_HTTPOPT_SSLCERTTYPE = SVNHTTPTYPE_OBJECTPOINT + 86,

	/*
	* String that holds file name of the SSL key to use
	*/
	SVN_HTTPOPT_SSLKEY = SVNHTTPTYPE_OBJECTPOINT + 87,

	/*
	* String that holds file type of the SSL key to use
	*/
	SVN_HTTPOPT_SSLKEYTYPE = SVNHTTPTYPE_OBJECTPOINT + 88,

	/*
	* String that holds the SSL or SSH private key password.
	*/
	SVN_HTTPOPT_KEYPASSWD = SVNHTTPTYPE_OBJECTPOINT + 26,

	/*
	* String that holds the SSL crypto engine.
	*/
	SVN_HTTPOPT_SSLENGINE = SVNHTTPTYPE_OBJECTPOINT + 89,

	/*
	* flag to set engine as default.
	*/
	SVN_HTTPOPT_SSLENGINE_DEFAULT = SVNHTTPTYPE_LONG + 90,

	/*
	* Kludgy option to enable CRLF conversions. Subject for removal.
	*/
	SVN_HTTPOPT_CRLF = SVNHTTPTYPE_LONG + 27,

	/*
	* Set what interface or address/hostname to bind the socket to when
	* performing an operation and thus what from-IP your connection will use.
	*/
	SVN_HTTPOPT_INTERFACE = SVNHTTPTYPE_OBJECTPOINT + 62,

	/*
	* Set what local port to bind the socket to when performing an operation.
	*/
	SVN_HTTPOPT_LOCALPORT = SVNHTTPTYPE_LONG + 139,

	/*
	* Set number of local ports to try, starting with SVN_HTTPOPT_LOCALPORT.
	*/
	SVN_HTTPOPT_LOCALPORTRANGE = SVNHTTPTYPE_LONG + 140,

	/*
	* A string that defines the kerberos security level.
	*/
	SVN_HTTPOPT_KRBLEVEL = SVNHTTPTYPE_OBJECTPOINT + 63,

	/*
	* Enable peer SSL verifying.
	*/
	SVN_HTTPOPT_SSL_VERIFYPEER = SVNHTTPTYPE_LONG + 64,

	/*
	* Enable verification of the CN contained in the peer certificate
	*/
	SVN_HTTPOPT_SSL_VERIFYHOST = SVNHTTPTYPE_LONG + 81,

	/* since these two options are only possible to use on an OpenSSL-
	powered libcurl we #ifdef them on this condition so that libcurls
	built against other SSL libs will return a proper error when trying
	to set this option! */
	/*
	* Set a SSL_CTX callback
	*/
	SVN_HTTPOPT_SSL_CTX_FUNCTION = SVNHTTPTYPE_FUNCTIONPOINT + 108,

	/*
	* Set a SSL_CTX callback parameter pointer
	*/
	SVN_HTTPOPT_SSL_CTX_DATA = SVNHTTPTYPE_OBJECTPOINT + 109,

	SVN_HTTPOPT_CERTINFO = SVNHTTPTYPE_LONG + 172,

	/*
	* Set CA info for SSL connection. Specify file name of the CA certificate
	*/
	SVN_HTTPOPT_CAINFO = SVNHTTPTYPE_OBJECTPOINT + 65,

	/*
	* Set CA path info for SSL connection. Specify directory name of the CA
	* certificates which have been prepared using openssl c_rehash utility.
	*/
	/* This does not work on windows. */
	SVN_HTTPOPT_CAPATH = SVNHTTPTYPE_OBJECTPOINT + 97,

	/*
	* Set CRL file info for SSL connection. Specify file name of the CRL
	* to check certificates revocation
	*/
	SVN_HTTPOPT_CRLFILE = SVNHTTPTYPE_OBJECTPOINT + 169,

	/*
	* Set Issuer certificate file
	* to check certificates issuer
	*/
	SVN_HTTPOPT_ISSUERCERT = SVNHTTPTYPE_OBJECTPOINT + 170,

	/*
	* Set a linked list of remote login options
	*/
	SVN_HTTPOPT_TELNETOPTIONS = SVNHTTPTYPE_OBJECTPOINT + 70,

	/*
	* The application kindly asks for a differently sized receive buffer.
	* If it seems reasonable, we'll use it.
	*/
	SVN_HTTPOPT_BUFFERSIZE = SVNHTTPTYPE_LONG + 98,

       SVN_HTTPOPT_SHARE = SVNHTTPTYPE_OBJECTPOINT + 100,


       SVN_HTTPOPT_PRIVATE = SVNHTTPTYPE_OBJECTPOINT + 103,

    

	/*
	* Set the maximum size of a file to download.
	*/
	SVN_HTTPOPT_MAXFILESIZE = SVNHTTPTYPE_LONG + 114,


	/*
	* Make transfers attempt to use SSL/TLS.
	*/
	SVN_HTTPOPT_USE_SSL = SVNHTTPTYPE_LONG + 119,

	/*
	* Set a specific auth for FTP-SSL transfers.
	*/
	SVN_HTTPOPT_FTPSSLAUTH = SVNHTTPTYPE_LONG + 129,


	SVN_HTTPOPT_IPRESOLVE = SVNHTTPTYPE_LONG + 113,

	/*
	* Set the maximum size of a file to download.
	*/
	SVN_HTTPOPT_MAXFILESIZE_LARGE = SVNHTTPTYPE_OFF_T + 117,

	/*
	* Enable or disable TCP_NODELAY, which will disable/enable the Nagle
	* algorithm
	*/
	SVN_HTTPOPT_TCP_NODELAY = SVNHTTPTYPE_LONG + 121,


	SVN_HTTPOPT_FTP_ACCOUNT = SVNHTTPTYPE_OBJECTPOINT + 134,

	SVN_HTTPOPT_IGNORE_CONTENT_LENGTH = SVNHTTPTYPE_LONG + 136,

	/*
	* No data transfer, set up connection and let application use the socket
	*/
	SVN_HTTPOPT_CONNECT_ONLY = SVNHTTPTYPE_LONG + 141,


	SVN_HTTPOPT_FTP_ALTERNATIVE_TO_USER = SVNHTTPTYPE_OBJECTPOINT + 147,

	/*
	* socket callback function: called after socket() but before connect()
	*/
	SVN_HTTPOPT_SOCKOPTFUNCTION = SVNHTTPTYPE_FUNCTIONPOINT + 148,

	/*
	* socket callback data pointer. Might be NULL.
	*/
	SVN_HTTPOPT_SOCKOPTDATA = SVNHTTPTYPE_OBJECTPOINT + 149,

	/*
	* open/create socket callback function: called instead of socket(),
	* before connect()
	*/
	SVN_HTTPOPT_OPENSOCKETFUNCTION = SVNHTTPTYPE_FUNCTIONPOINT + 163,

	/*
	* socket callback data pointer. Might be NULL.
	*/
	SVN_HTTPOPT_OPENSOCKETDATA = SVNHTTPTYPE_OBJECTPOINT + 164,

	/*
	* close socket callback function: called instead of close()
	* when shutting down a connection
	*/
	SVN_HTTPOPT_CLOSESOCKETFUNCTION = SVNHTTPTYPE_FUNCTIONPOINT + 208,

	/*
	* socket callback data pointer. Might be NULL.
	*/
	SVN_HTTPOPT_CLOSESOCKETDATA = SVNHTTPTYPE_OBJECTPOINT + 209,


	SVN_HTTPOPT_SSL_SESSIONID_CACHE = SVNHTTPTYPE_LONG + 150,


	SVN_HTTPOPT_SSH_AUTH_TYPES = SVNHTTPTYPE_LONG + 151,

	/*
	* Use this file instead of the $HOME/.ssh/id_dsa.pub file
	*/
	SVN_HTTPOPT_SSH_PUBLIC_KEYFILE = SVNHTTPTYPE_OBJECTPOINT + 152,

	/*
	* Use this file instead of the $HOME/.ssh/id_dsa file
	*/
	SVN_HTTPOPT_SSH_PRIVATE_KEYFILE = SVNHTTPTYPE_OBJECTPOINT + 153,

	/*
	* Option to allow for the MD5 of the host public key to be checked
	* for validation purposes.
	*/
	SVN_HTTPOPT_SSH_HOST_PUBLIC_KEY_MD5 = SVNHTTPTYPE_OBJECTPOINT + 162,

	/*
	* Store the file name to read known hosts from.
	*/
	SVN_HTTPOPT_SSH_KNOWNHOSTS = SVNHTTPTYPE_OBJECTPOINT + 183,

	/* setting to NULL is fine since the ssh.c functions themselves will
	then rever to use the internal default */
	SVN_HTTPOPT_SSH_KEYFUNCTION = SVNHTTPTYPE_FUNCTIONPOINT + 184,

	/*
	* Custom client data to pass to the SSH keyfunc callback
	*/
	SVN_HTTPOPT_SSH_KEYDATA = SVNHTTPTYPE_OBJECTPOINT + 185,

	/*
	* disable libcurl transfer encoding is used
	*/
	SVN_HTTPOPT_HTTP_TRANSFER_DECODING = SVNHTTPTYPE_LONG + 157,

	/*
	* raw data passed to the application when content encoding is used
	*/
	SVN_HTTPOPT_HTTP_CONTENT_DECODING = SVNHTTPTYPE_LONG + 158,

	/*
	* Uses these permissions instead of 0644
	*/
	SVN_HTTPOPT_NEW_FILE_PERMS = SVNHTTPTYPE_LONG + 159,

	/*
	* Uses these permissions instead of 0755
	*/
	SVN_HTTPOPT_NEW_DIRECTORY_PERMS = SVNHTTPTYPE_LONG + 160,

	/*
	* We always get longs when passed plain numericals, but for this value we
	* know that an unsigned int will always hold the value so we blindly
	* typecast to this type
	*/
	SVN_HTTPOPT_ADDRESS_SCOPE = SVNHTTPTYPE_LONG + 171,

	/* set the bitmask for the protocols that are allowed to be used for the
	transfer, which thus helps the app which takes URLs from users or other
	external inputs and want to restrict what protocol(s) to deal
	with. Defaults to SVNPROTO_ALL. */
	SVN_HTTPOPT_PROTOCOLS = SVNHTTPTYPE_LONG + 181,

	/* set the bitmask for the protocols that libcurl is allowed to follow to,
	as a subset of the SVN_HTTPOPT_PROTOCOLS ones. That means the protocol needs
	to be set in both bitmasks to be allowed to get redirected to. Defaults
	to all protocols except FILE and SCP. */
	SVN_HTTPOPT_REDIR_PROTOCOLS = SVNHTTPTYPE_LONG + 182,


	SVN_HTTPOPT_MAIL_FROM = SVNHTTPTYPE_OBJECTPOINT + 186,

	/* get a list of mail recipients */
	SVN_HTTPOPT_MAIL_RCPT = SVNHTTPTYPE_OBJECTPOINT + 187,

	/*
	* Set the RTSP request method (OPTIONS, SETUP, PLAY, etc...)
	* Would this be better if the RTSPREQ_* were just moved into here?
	*/
	SVN_HTTPOPT_RTSP_REQUEST = SVNHTTPTYPE_LONG + 189,//=============================================;


	/*
	* Set the RTSP Session ID manually. Useful if the application is
	* resuming a previously established RTSP session
	*/
	SVN_HTTPOPT_RTSP_SESSION_ID = SVNHTTPTYPE_OBJECTPOINT + 190,

	/*
	* Set the Stream URI for the RTSP request. Unless the request is
	* for generic server options, the application will need to set this.
	*/
	SVN_HTTPOPT_RTSP_STREAM_URI = SVNHTTPTYPE_OBJECTPOINT + 191,

	/*
	* The content of the Transport: header for the RTSP request
	*/
	SVN_HTTPOPT_RTSP_TRANSPORT = SVNHTTPTYPE_OBJECTPOINT + 192,

	/*
	* Set the CSEQ number to issue for the next RTSP request. Useful if the
	* application is resuming a previously broken connection. The CSEQ
	* will increment from this new number henceforth.
	*/
	SVN_HTTPOPT_RTSP_CLIENT_CSEQ = SVNHTTPTYPE_LONG + 193,


	/* Same as the above, but for server-initiated requests */
	SVN_HTTPOPT_RTSP_SERVER_CSEQ = SVNHTTPTYPE_LONG + 194,


	SVN_HTTPOPT_INTERLEAVEDATA = SVNHTTPTYPE_OBJECTPOINT + 195,

	/* Set the user defined RTP write function */
	SVN_HTTPOPT_INTERLEAVEFUNCTION = SVNHTTPTYPE_FUNCTIONPOINT + 196,


	SVN_HTTPOPT_WILDCARDMATCH = SVNHTTPTYPE_LONG + 197,

	SVN_HTTPOPT_CHUNK_BGN_FUNCTION = SVNHTTPTYPE_FUNCTIONPOINT + 198,


	SVN_HTTPOPT_CHUNK_END_FUNCTION = SVNHTTPTYPE_FUNCTIONPOINT + 199,


	SVN_HTTPOPT_FNMATCH_FUNCTION = SVNHTTPTYPE_FUNCTIONPOINT + 200,

	SVN_HTTPOPT_CHUNK_DATA = SVNHTTPTYPE_OBJECTPOINT + 201,


	SVN_HTTPOPT_FNMATCH_DATA = SVNHTTPTYPE_OBJECTPOINT + 202,


	SVN_HTTPOPT_TLSAUTH_USERNAME = SVNHTTPTYPE_OBJECTPOINT + 204,


	SVN_HTTPOPT_TLSAUTH_PASSWORD = SVNHTTPTYPE_OBJECTPOINT + 205,


	SVN_HTTPOPT_TLSAUTH_TYPE = SVNHTTPTYPE_OBJECTPOINT + 206,

	SVN_HTTPOPT_DNS_SERVERS = SVNHTTPTYPE_OBJECTPOINT + 211,

	/*	多媒体隧道控制选项  */
	SVN_HTTPOPT_MTM = SVNHTTPTYPE_POINT + 1,	

	/*Add by yangjiemiao yKF70953 2012-08-23 增加options操作*/
	SVN_HTTPOPT_OPTIONS = SVNHTTPTYPE_POINT + 2,

    /* BEGIN: Added for PN:TCP over UDP 整改 by y90004712, 2013/8/16 */
    SVN_HTTPOPT_TUNNEL_TYPE = SVNHTTPTYPE_POINT + 3,
    /* END:   Added for PN:TCP over UDP 整改 by y90004712, 2013/8/16 */

    /* BEGIN: Added for PN:增加http交互取消机制 by y90004712, 2013/11/1 */
    SVN_HTTPOPT_CANCEL_CALLBACK = SVNHTTPTYPE_POINT + 4,
    SVN_HTTPOPT_CANCEL_DATA = SVNHTTPTYPE_POINT + 5,
    /* END:   Added for PN:增加http交互取消机制 by y90004712, 2013/11/1 */
    SVN_HTTPOPT_HOST_NAME = SVNHTTPTYPE_POINT + 6
}SVN_HTTPOPT;

#define SVNINFO_STRING	0x100000
#define SVNINFO_LONG	0x200000
#define SVNINFO_DOUBLE	0x300000
#define SVNINFO_SLIST   0x400000

/*svn_http_getinfo 返回信息选项*/
typedef enum  
{
	SVN_HTTPINFO_NONE, /* first, never use this */
	SVN_HTTPINFO_EFFECTIVE_URL				= SVNINFO_STRING + 1,
	SVN_HTTPINFO_RESPONSE_CODE				= SVNINFO_LONG   + 2,
	SVN_HTTPINFO_TOTAL_TIME					= SVNINFO_DOUBLE + 3,
	SVN_HTTPINFO_NAMELOOKUP_TIME			= SVNINFO_DOUBLE + 4,
	SVN_HTTPINFO_CONNECT_TIME				= SVNINFO_DOUBLE + 5,
	SVN_HTTPINFO_PRETRANSFER_TIME			= SVNINFO_DOUBLE + 6,
	SVN_HTTPINFO_SIZE_UPLOAD				= SVNINFO_DOUBLE + 7,
	SVN_HTTPINFO_SIZE_DOWNLOAD				= SVNINFO_DOUBLE + 8,
	SVN_HTTPINFO_SPEED_DOWNLOAD				= SVNINFO_DOUBLE + 9,
	SVN_HTTPINFO_SPEED_UPLOAD				= SVNINFO_DOUBLE + 10,
	SVN_HTTPINFO_HEADER_SIZE				= SVNINFO_LONG   + 11,
	SVN_HTTPINFO_REQUEST_SIZE				= SVNINFO_LONG   + 12,
	SVN_HTTPINFO_SSL_VERIFYRESULT			= SVNINFO_LONG   + 13,
	SVN_HTTPINFO_FILETIME					= SVNINFO_LONG   + 14,
	SVN_HTTPINFO_CONTENT_LENGTH_DOWNLOAD	= SVNINFO_DOUBLE + 15,
	SVN_HTTPINFO_CONTENT_LENGTH_UPLOAD		= SVNINFO_DOUBLE + 16,
	SVN_HTTPINFO_STARTTRANSFER_TIME			= SVNINFO_DOUBLE + 17,
	SVN_HTTPINFO_CONTENT_TYPE				= SVNINFO_STRING + 18,
	SVN_HTTPINFO_REDIRECT_TIME				= SVNINFO_DOUBLE + 19,
	SVN_HTTPINFO_REDIRECT_COUNT				= SVNINFO_LONG   + 20,
	SVN_HTTPINFO_PRIVATE					= SVNINFO_STRING + 21,
	SVN_HTTPINFO_HTTP_CONNECTCODE			= SVNINFO_LONG   + 22,
	SVN_HTTPINFO_HTTPAUTH_AVAIL				= SVNINFO_LONG   + 23,
	SVN_HTTPINFO_PROXYAUTH_AVAIL			= SVNINFO_LONG   + 24,
	SVN_HTTPINFO_OS_ERRNO					= SVNINFO_LONG   + 25,
	SVN_HTTPINFO_NUM_CONNECTS				= SVNINFO_LONG   + 26,
	SVN_HTTPINFO_SSL_ENGINES				= SVNINFO_SLIST  + 27,
	SVN_HTTPINFO_COOKIELIST					= SVNINFO_SLIST  + 28,
	SVN_HTTPINFO_LASTSOCKET					= SVNINFO_LONG   + 29,
	SVN_HTTPINFO_FTP_ENTRY_PATH				= SVNINFO_STRING + 30,
	SVN_HTTPINFO_REDIRECT_URL				= SVNINFO_STRING + 31,
	SVN_HTTPINFO_PRIMARY_IP					= SVNINFO_STRING + 32,
	SVN_HTTPINFO_APPCONNECT_TIME			= SVNINFO_DOUBLE + 33,
	SVN_HTTPINFO_CERTINFO					= SVNINFO_SLIST  + 34,
	SVN_HTTPINFO_CONDITION_UNMET			= SVNINFO_LONG   + 35,
	SVN_HTTPINFO_RTSP_SESSION_ID			= SVNINFO_STRING + 36,
	SVN_HTTPINFO_RTSP_CLIENT_CSEQ			= SVNINFO_LONG   + 37,
	SVN_HTTPINFO_RTSP_SERVER_CSEQ			= SVNINFO_LONG   + 38,
	SVN_HTTPINFO_RTSP_CSEQ_RECV				= SVNINFO_LONG   + 39,
	SVN_HTTPINFO_PRIMARY_PORT				= SVNINFO_LONG   + 40,
	SVN_HTTPINFO_LOCAL_IP					= SVNINFO_STRING + 41,
	SVN_HTTPINFO_LOCAL_PORT					= SVNINFO_LONG   + 42,

	SVN_HTTPINFO_LASTONE					= 42

}SVN_HTTPINFO;

//请求返回的信息存储回调函数，即使用了SVN_HTTPOPT_WRITEFUNCTION
typedef unsigned long WriteFunctionCallBack( char *ptr, unsigned long size, unsigned long nmemb, void *userdata);

//请求返回信息的http头信息存储回调函数，即使用了SVN_HTTPOPT_HEADERFUNCTION
typedef unsigned long HeaderFunctionCallBack( char *ptr, unsigned long size, unsigned long nmemb, void *userdata);

//读取本地数据回调函数，即使用了SVN_HTTPOPT_READFUNCTION
typedef unsigned long ReadFunctionCallBack(char *ptr, unsigned long size,unsigned long nitemb, void *userdata);

//进度条控制回调函数，即使用了SVN_HTTPOPT_PROGRESSFUNCTION
typedef int SvnhttpProgressCallBack(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);

/* This prototype applies to all conversion callbacks */
typedef int SvnConvCallBack(char *ptr, unsigned long length);


typedef enum {
	SVNHTTPIOE_OK,            /* I/O operation successful */
	SVNHTTPIOE_UNKNOWNCMD,    /* command was unknown to callback */
	SVNHTTPIOE_FAILRESTART,   /* failed to restart the read */
	SVNHTTPIOE_LAST           /* never use */
} svnhttpioerr;

typedef enum  {
	SVNHTTPIOCMD_NOP,         /* no operation */
	SVNHTTPIOCMD_RESTARTREAD, /* restart the read stream from start */
	SVNHTTPIOCMD_LAST         /* never use */
} svnhttpiocmd;

//I/O控制回调函数，即使用了SVN_HTTPOPT_IOCTLFUNCTION
typedef svnhttpioerr SvnIoctlFunctionCallBack(SvnHttp* pHttpHandle, int cmd, void *clientp);

//ctx回调函数，即使用了 CURLOPT_SSL_CTX_FUNCTION
typedef int Svnssl_ctx_callback(SvnHttp* pHttpHandle, 
								void *ssl_ctx, /* actually an OpenSSL SSL_CTX */
								void *userptr );

typedef enum  {
	SVNHTTPSOCKTYPE_IPCXN, /* socket created for a specific IP connection */
	SVNHTTPSOCKTYPE_LAST   /* never use */
} svnhttpsocktype;

#define SVNHTTP_SOCKOPT_OK 0
#define SVNHTTP_SOCKOPT_ERROR 1 /* causes libcurl to abort and return CURLE_ABORTED_BY_CALLBACK */
#define SVNHTTP_SOCKOPT_ALREADY_CONNECTED 2

/*	指定 SVN_HTTPOPT_SOCKOPTFUNCTION 选项时的回调函数  */
typedef int SvnSockOptFunctionCallBack(void *clientp, int curlfd, svnhttpsocktype purpose);

struct svnhttp_sockaddr {
	int family;
	int socktype;
	int protocol;
	unsigned int addrlen; 
	struct sockaddr addr;
};

/*	指定 SVN_HTTPOPT_OPENSOCKETFUNCTION 选项时的回调函数  */
typedef int SvnOpenSocketFunctionCallBack (void *clientp, svnhttpsocktype purpose, struct svnhttp_sockaddr *address);

/*	指定 SVN_HTTPOPT_CLOSESOCKETFUNCTION 选项时的回调函数  */
typedef int SvnCloseSocketFunctionCallBack(void *clientp, int item);


/* return codes for SVN_HTTPOPT_CHUNK_BGN_FUNCTION */
#define SVNHTTP_CHUNK_BGN_FUNC_OK      0
#define SVNHTTP_CHUNK_BGN_FUNC_FAIL    1 /* tell the lib to end the task */
#define SVNHTTP_CHUNK_BGN_FUNC_SKIP    2 /* skip this chunk over */

/* if splitting of data transfer is enabled, this callback is called before
download of an individual chunk started. Note that parameter "remains" works
only for FTP wildcard downloading (for now), otherwise is not used */
typedef long SvnChunkBgnFunction(const void *transfer_info, void *ptr, int remains);


/* return codes for SVN_HTTPOPT_CHUNK_END_FUNCTIONN */
#define CURL_CHUNK_END_FUNC_OK      0
#define CURL_CHUNK_END_FUNC_FAIL    1 /* tell the lib to end the task */

/* If splitting of data transfer is enabled this callback is called after
download of an individual chunk finished.
Note! After this callback was set then it have to be called FOR ALL chunks.
Even if downloading of this chunk was skipped in CHUNK_BGN_FUNC.
This is the reason why we don't need "transfer_info" parameter in this
callback and we are not interested in "remains" parameter too. */
typedef long SvnChunkEndFunction(void *ptr);

/* return codes for SVN_HTTPOPT_FNMATCH_FUNCTION */
#define CURL_FNMATCHFUNC_MATCH    0 /* string corresponds to the pattern */
#define CURL_FNMATCHFUNC_NOMATCH  1 /* pattern doesn't match the string */
#define CURL_FNMATCHFUNC_FAIL     2 /* an error occurred */

/* callback type for wildcard downloading pattern matching. If the
string matches the pattern, return SVN_HTTPOPT_FNMATCH_FUNCTION value, etc. */
typedef int SvnFnMatchFunction(void *ptr, const char *pattern, const char *string);


/* the kind of data that is passed to information_callback*/
typedef enum {
	SVNHTTPINFO_TEXT = 0,
	SVNHTTPINFO_HEADER_IN,    /* 1 */
	SVNHTTPINFO_HEADER_OUT,   /* 2 */
	SVNHTTPINFO_DATA_IN,      /* 3 */
	SVNHTTPINFO_DATA_OUT,     /* 4 */
	SVNHTTPINFO_SSL_DATA_IN,  /* 5 */
	SVNHTTPINFO_SSL_DATA_OUT, /* 6 */
	SVNHTTPINFO_END
} svnhttp_infotype;

/*	指定 SVN_HTTPOPT_DEBUGFUNCTION 选项时的回调函数  */
typedef int SvnDebugFunction ( SvnHttp *handle,       /* the handle/transfer this concerns */
							   svnhttp_infotype type, /* what kind of data */
							   char *data,            /* points to the data */
							   unsigned long size,     /* size of the data pointed to */
							   void *userptr);        /* whatever the user please */

/* These are the return codes for the seek callbacks */
#define CURL_SEEKFUNC_OK       0
#define CURL_SEEKFUNC_FAIL     1 /* fail the entire transfer */
#define CURL_SEEKFUNC_CANTSEEK 2 /* tell libcurl seeking can't be done, so libcurl might try other means instead */
/*	指定 CURLOPT_SEEKFUNCTION 选项时的回调函数  */
typedef int SvnSeekFunction(void *instream, long offset, int origin); /* 'whence' */


typedef enum {
  svn_http_CURLM_CALL_MULTI_PERFORM = -1, /* please call curl_multi_perform() or
                                    curl_multi_socket*() soon */
  svn_http_CURLM_OK,
  svn_http_CURLM_BAD_HANDLE,      /* the passed-in handle is not a valid CURLM handle */
  svn_http_CURLM_BAD_EASY_HANDLE, /* an easy handle was not good/valid */
  svn_http_CURLM_OUT_OF_MEMORY,   /* if you ever get this, you're in deep sh*t */
  svn_http_CURLM_INTERNAL_ERROR,  /* this is a libcurl bug */
  svn_http_CURLM_BAD_SOCKET,      /* the passed in socket argument did not match */
  svn_http_CURLM_UNKNOWN_OPTION,  /* curl_multi_setopt() with unsupported option */
  svn_http_CURLM_LAST
} svn_http_CURLMcode;

typedef void svn_http_CURLM;

typedef enum {
  svn_http_CURLSHE_OK,  /* all is fine */
  svn_http_CURLSHE_BAD_OPTION, /* 1 */
  svn_http_CURLSHE_IN_USE,     /* 2 */
  svn_http_CURLSHE_INVALID,    /* 3 */
  svn_http_CURLSHE_NOMEM,      /* 4 out of memory */
  svn_http_CURLSHE_NOT_BUILT_IN, /* 5 feature not present in lib */
  svn_http_CURLSHE_LAST        /* never use */
} svn_http_CURLSHcode;

typedef void svn_http_CURLSH;

typedef enum {
  svn_http_CURLE_OK = 0,
  svn_http_CURLE_UNSUPPORTED_PROTOCOL,    /* 1 */
  svn_http_CURLE_FAILED_INIT,             /* 2 */
  svn_http_CURLE_URL_MALFORMAT,           /* 3 */
  svn_http_CURLE_NOT_BUILT_IN,            /* 4 - [was obsoleted in August 2007 for
                                    7.17.0, reused in April 2011 for 7.21.5] */
  svn_http_CURLE_COULDNT_RESOLVE_PROXY,   /* 5 */
  svn_http_CURLE_COULDNT_RESOLVE_HOST,    /* 6 */
  svn_http_CURLE_COULDNT_CONNECT,         /* 7 */
  svn_http_CURLE_FTP_WEIRD_SERVER_REPLY,  /* 8 */
  svn_http_CURLE_REMOTE_ACCESS_DENIED,    /* 9 a service was denied by the server
                                    due to lack of access - when login fails
                                    this is not returned. */
  svn_http_CURLE_FTP_ACCEPT_FAILED,       /* 10 - [was obsoleted in April 2006 for
                                    7.15.4, reused in Dec 2011 for 7.24.0]*/
  svn_http_CURLE_FTP_WEIRD_PASS_REPLY,    /* 11 */
  svn_http_CURLE_FTP_ACCEPT_TIMEOUT,      /* 12 - timeout occurred accepting server
                                    [was obsoleted in August 2007 for 7.17.0,
                                    reused in Dec 2011 for 7.24.0]*/
  svn_http_CURLE_FTP_WEIRD_PASV_REPLY,    /* 13 */
  svn_http_CURLE_FTP_WEIRD_227_FORMAT,    /* 14 */
  svn_http_CURLE_FTP_CANT_GET_HOST,       /* 15 */
  svn_http_CURLE_OBSOLETE16,              /* 16 - NOT USED */
  svn_http_CURLE_FTP_COULDNT_SET_TYPE,    /* 17 */
  svn_http_CURLE_PARTIAL_FILE,            /* 18 */
  svn_http_CURLE_FTP_COULDNT_RETR_FILE,   /* 19 */
  svn_http_CURLE_OBSOLETE20,              /* 20 - NOT USED */
  svn_http_CURLE_QUOTE_ERROR,             /* 21 - quote command failure */
  svn_http_CURLE_HTTP_RETURNED_ERROR,     /* 22 */
  svn_http_CURLE_WRITE_ERROR,             /* 23 */
  svn_http_CURLE_OBSOLETE24,              /* 24 - NOT USED */
  svn_http_CURLE_UPLOAD_FAILED,           /* 25 - failed upload "command" */
  svn_http_CURLE_READ_ERROR,              /* 26 - couldn't open/read from file */
  svn_http_CURLE_OUT_OF_MEMORY,           /* 27 */
  /* Note: svn_http_CURLE_OUT_OF_MEMORY may sometimes indicate a conversion error
           instead of a memory allocation error if CURL_DOES_CONVERSIONS
           is defined
  */
  svn_http_CURLE_OPERATION_TIMEDOUT,      /* 28 - the timeout time was reached */
  svn_http_CURLE_OBSOLETE29,              /* 29 - NOT USED */
  svn_http_CURLE_FTP_PORT_FAILED,         /* 30 - FTP PORT operation failed */
  svn_http_CURLE_FTP_COULDNT_USE_REST,    /* 31 - the REST command failed */
  svn_http_CURLE_OBSOLETE32,              /* 32 - NOT USED */
  svn_http_CURLE_RANGE_ERROR,             /* 33 - RANGE "command" didn't work */
  svn_http_CURLE_HTTP_POST_ERROR,         /* 34 */
  svn_http_CURLE_SSL_CONNECT_ERROR,       /* 35 - wrong when connecting with SSL */
  svn_http_CURLE_BAD_DOWNLOAD_RESUME,     /* 36 - couldn't resume download */
  svn_http_CURLE_FILE_COULDNT_READ_FILE,  /* 37 */
  svn_http_CURLE_LDAP_CANNOT_BIND,        /* 38 */
  svn_http_CURLE_LDAP_SEARCH_FAILED,      /* 39 */
  svn_http_CURLE_OBSOLETE40,              /* 40 - NOT USED */
  svn_http_CURLE_FUNCTION_NOT_FOUND,      /* 41 */
  svn_http_CURLE_ABORTED_BY_CALLBACK,     /* 42 */
  svn_http_CURLE_BAD_FUNCTION_ARGUMENT,   /* 43 */
  svn_http_CURLE_OBSOLETE44,              /* 44 - NOT USED */
  svn_http_CURLE_INTERFACE_FAILED,        /* 45 - CURLOPT_INTERFACE failed */
  svn_http_CURLE_OBSOLETE46,              /* 46 - NOT USED */
  svn_http_CURLE_TOO_MANY_REDIRECTS ,     /* 47 - catch endless re-direct loops */
  svn_http_CURLE_UNKNOWN_OPTION,          /* 48 - User specified an unknown option */
  svn_http_CURLE_TELNET_OPTION_SYNTAX ,   /* 49 - Malformed remote login option */
  svn_http_CURLE_OBSOLETE50,              /* 50 - NOT USED */
  svn_http_CURLE_PEER_FAILED_VERIFICATION, /* 51 - peer's certificate or fingerprint
                                     wasn't verified fine */
  svn_http_CURLE_GOT_NOTHING,             /* 52 - when this is a specific error */
  svn_http_CURLE_SSL_ENGINE_NOTFOUND,     /* 53 - SSL crypto engine not found */
  svn_http_CURLE_SSL_ENGINE_SETFAILED,    /* 54 - can not set SSL crypto engine as
                                    default */
  svn_http_CURLE_SEND_ERROR,              /* 55 - failed sending network data */
  svn_http_CURLE_RECV_ERROR,              /* 56 - failure in receiving network data */
  svn_http_CURLE_OBSOLETE57,              /* 57 - NOT IN USE */
  svn_http_CURLE_SSL_CERTPROBLEM,         /* 58 - problem with the local certificate */
  svn_http_CURLE_SSL_CIPHER,              /* 59 - couldn't use specified cipher */
  svn_http_CURLE_SSL_CACERT,              /* 60 - problem with the CA cert (path?) */
  svn_http_CURLE_BAD_CONTENT_ENCODING,    /* 61 - Unrecognized/bad encoding */
  svn_http_CURLE_LDAP_INVALID_URL,        /* 62 - Invalid LDAP URL */
  svn_http_CURLE_FILESIZE_EXCEEDED,       /* 63 - Maximum file size exceeded */
  svn_http_CURLE_USE_SSL_FAILED,          /* 64 - Requested FTP SSL level failed */
  svn_http_CURLE_SEND_FAIL_REWIND,        /* 65 - Sending the data requires a rewind
                                    that failed */
  svn_http_CURLE_SSL_ENGINE_INITFAILED,   /* 66 - failed to initialise ENGINE */
  svn_http_CURLE_LOGIN_DENIED,            /* 67 - user, password or similar was not
                                    accepted and we failed to login */
  svn_http_CURLE_TFTP_NOTFOUND,           /* 68 - file not found on server */
  svn_http_CURLE_TFTP_PERM,               /* 69 - permission problem on server */
  svn_http_CURLE_REMOTE_DISK_FULL,        /* 70 - out of disk space on server */
  svn_http_CURLE_TFTP_ILLEGAL,            /* 71 - Illegal TFTP operation */
  svn_http_CURLE_TFTP_UNKNOWNID,          /* 72 - Unknown transfer ID */
  svn_http_CURLE_REMOTE_FILE_EXISTS,      /* 73 - File already exists */
  svn_http_CURLE_TFTP_NOSUCHUSER,         /* 74 - No such user */
  svn_http_CURLE_CONV_FAILED,             /* 75 - conversion failed */
  svn_http_CURLE_CONV_REQD,               /* 76 - caller must register conversion
                                    callbacks using curl_easy_setopt options
                                    CURLOPT_CONV_FROM_NETWORK_FUNCTION,
                                    CURLOPT_CONV_TO_NETWORK_FUNCTION, and
                                    CURLOPT_CONV_FROM_UTF8_FUNCTION */
  svn_http_CURLE_SSL_CACERT_BADFILE,      /* 77 - could not load CACERT file, missing
                                    or wrong format */
  svn_http_CURLE_REMOTE_FILE_NOT_FOUND,   /* 78 - remote file not found */
  svn_http_CURLE_SSH,                     /* 79 - error from the SSH layer, somewhat
                                    generic so the error message will be of
                                    interest when this has happened */

  svn_http_CURLE_SSL_SHUTDOWN_FAILED,     /* 80 - Failed to shut down the SSL
                                    connection */
  svn_http_CURLE_AGAIN,                   /* 81 - socket is not ready for send/recv,
                                    wait till it's ready and try again (Added
                                    in 7.18.2) */
  svn_http_CURLE_SSL_CRL_BADFILE,         /* 82 - could not load CRL file, missing or
                                    wrong format (Added in 7.19.0) */
  svn_http_CURLE_SSL_ISSUER_ERROR,        /* 83 - Issuer check failed.  (Added in
                                    7.19.0) */
  svn_http_CURLE_FTP_PRET_FAILED,         /* 84 - a PRET command failed */
  svn_http_CURLE_RTSP_CSEQ_ERROR,         /* 85 - mismatch of RTSP CSeq numbers */
  svn_http_CURLE_RTSP_SESSION_ERROR,      /* 86 - mismatch of RTSP Session Ids */
  svn_http_CURLE_FTP_BAD_FILE_LIST,       /* 87 - unable to parse FTP file list */
  svn_http_CURLE_CHUNK_FAILED,            /* 88 - chunk callback reported error */

  svn_http_CURL_LAST /* never use! */
} svn_http_CURLcode;

typedef void svn_http_CURL;



typedef enum {
  svn_http_CURLMSG_NONE, /* first, not used */
  svn_http_CURLMSG_DONE, /* This easy handle has completed. 'result' contains
                   the CURLcode of the transfer */
  svn_http_CURLMSG_LAST /* last, not used */
} svn_http_CURLMSG;

struct svn_http_CURLMsg {
  svn_http_CURLMSG msg;       /* what this message means */
  svn_http_CURL *easy_handle; /* the handle it concerns */
  union {
    void *whatever;    /* message-specific data */
    svn_http_CURLcode result;   /* return code for transfer */
  } data;
};
typedef struct svn_http_CURLMsg svn_http_CURLMsg;

typedef enum {
  svn_http_CURLSHOPT_NONE,  /* don't use */
  svn_http_CURLSHOPT_SHARE,   /* specify a data type to share */
  svn_http_CURLSHOPT_UNSHARE, /* specify which data type to stop sharing */
  svn_http_CURLSHOPT_LOCKFUNC,   /* pass in a 'curl_lock_function' pointer */
  svn_http_CURLSHOPT_UNLOCKFUNC, /* pass in a 'curl_unlock_function' pointer */
  svn_http_CURLSHOPT_USERDATA,   /* pass in a user data pointer used in the lock/unlock
                           callback functions */
  svn_http_CURLSHOPT_LAST  /* never use */
} svn_http_CURLSHoption;


struct svn_http_SessionHandle {
  int fake;
};

typedef enum {
  svn_http_CURL_LOCK_DATA_NONE = 0,
  /*  CURL_LOCK_DATA_SHARE is used internally to say that
   *  the locking is just made to change the internal state of the share
   *  itself.
   */
  svn_http_CURL_LOCK_DATA_SHARE,
  svn_http_CURL_LOCK_DATA_COOKIE,
  svn_http_CURL_LOCK_DATA_DNS,
  svn_http_CURL_LOCK_DATA_SSL_SESSION,
  svn_http_CURL_LOCK_DATA_CONNECT,
  svn_http_CURL_LOCK_DATA_LAST
} svn_http_curl_lock_data;


typedef enum {
  svn_http_CURL_LOCK_ACCESS_NONE = 0,   /* unspecified action */
  svn_http_CURL_LOCK_ACCESS_SHARED = 1, /* for read perhaps */
  svn_http_CURL_LOCK_ACCESS_SINGLE = 2, /* for write perhaps */
  svn_http_CURL_LOCK_ACCESS_LAST        /* never use */
} svn_http_curl_lock_access;



/*接口函数*/

#define SVN_GLOBAL_SSL (1<<0)
#define SVN_GLOBAL_WIN32 (1<<1)
#define SVN_GLOBAL_ALL (SVN_GLOBAL_SSL|SVN_GLOBAL_WIN32)


#define svn_http_CURLPAUSE_RECV      (1<<0)
#define svn_http_CURLPAUSE_RECV_CONT (0)

#define svn_http_CURLPAUSE_SEND      (1<<2)
#define svn_http_CURLPAUSE_SEND_CONT (0)

#define svn_http_CURLPAUSE_ALL       (svn_http_CURLPAUSE_RECV|svn_http_CURLPAUSE_SEND)
#define svn_http_CURLPAUSE_CONT      (svn_http_CURLPAUSE_RECV_CONT|svn_http_CURLPAUSE_SEND_CONT)

/*BEGIN: Modified by caorongfei\ckf50034 for DTS2012092205464  2012-10-07*/  
#define SVN_L4VPN       1
#define SVN_NOT_L4VPN       0
/*END: Modified by caorongfei\ckf50034 for DTS2012092205464  2012-11-07*/ 

CURL_EXTERN int svn_global_init(long flags);

CURL_EXTERN void svn_global_cleanup();

CURL_EXTERN SvnHttp * svn_http_initHandle(void);

void svn_http_use_svnsdk(unsigned long ulUseSvnsdk);

CURL_EXTERN int svn_http_setopt(SvnHttp* pHttpHandle ,SVN_HTTPOPT httpOpt,...);

CURL_EXTERN int svn_http_perform( SvnHttp * pHttpHandle );

CURL_EXTERN void svn_http_cleanupHandle(SvnHttp *pHttpHandle);

CURL_EXTERN char * svn_http_strerror(int iErr);

CURL_EXTERN struct http_slist *svn_http_slist_append(struct http_slist *list, const char *data);

CURL_EXTERN void svn_http_slist_free(struct http_slist *list);

CURL_EXTERN int svn_http_getinfo(SvnHttp* pHttpHandle, SVN_HTTPINFO httpInfo, ...);

CURL_EXTERN int svn_http_recv(SvnHttp* pHttpHandle, void *buf, unsigned long buflen, unsigned long *recvsize);

CURL_EXTERN int svn_http_send(SvnHttp* pHttpHandle, const void *buf, unsigned long buflen, unsigned long *sendsize);


CURL_EXTERN void svn_http_disconnect(SvnHttp* pHttpHandle);



CURL_EXTERN svn_http_CURLMcode svn_http_multi_cleanup(svn_http_CURLM *multi_handle);

svn_http_CURLSHcode svn_http_share_cleanup(svn_http_CURLSH *sh);

void svn_http_global_cleanup(void);

svn_http_CURLcode svn_http_global_init(long flags);

svn_http_CURLM* svn_http_multi_init(void);

svn_http_CURLSH * svn_http_share_init(void);

svn_http_CURLMcode svn_http_multi_add_handle(svn_http_CURLM *multi_handle, svn_http_CURL  *easy_handle);

svn_http_CURLMcode svn_http_multi_remove_handle(svn_http_CURLM *multi_handle, svn_http_CURL  *easy_handle);

svn_http_CURLMcode svn_http_multi_fdset(svn_http_CURLM *multi_handle, fd_set *read_fd_set, fd_set *write_fd_set, fd_set *exc_fd_set, int *max_fd);

svn_http_CURLMcode svn_http_multi_perform(svn_http_CURLM *multi_handle, int *running_handles);

svn_http_CURLMsg* svn_http_multi_info_read(svn_http_CURLM *multi_handle, int *msgs_in_queue);

svn_http_CURLSHcode svn_http_share_setopt(svn_http_CURLSH *sh, svn_http_CURLSHoption option, ...);

svn_http_CURLcode svn_http_easy_pause(svn_http_CURL  *easy_handle, int action);

svn_http_CURLcode svn_http_share_lock(struct svn_http_SessionHandle *data, svn_http_curl_lock_data type, svn_http_curl_lock_access accesstype);

svn_http_CURLcode svn_http_share_unlock(struct svn_http_SessionHandle *data, svn_http_curl_lock_data type);


/* use this for multipart formpost building */
/* Returns code for curl_formadd()
*
* Returns:
* SVNHTTP_FORMADD_OK             on success
* SVNHTTP_FORMADD_MEMORY         if the FormInfo allocation fails
* SVNHTTP_FORMADD_OPTION_TWICE   if one option is given twice for one Form
* SVNHTTP_FORMADD_NULL           if a null pointer was given for a char
* SVNHTTP_FORMADD_MEMORY         if the allocation of a FormInfo struct failed
* SVNHTTP_FORMADD_UNKNOWN_OPTION if an unknown option was used
* SVNHTTP_FORMADD_INCOMPLETE     if the some FormInfo is not complete (or error)
* SVNHTTP_FORMADD_MEMORY         if a curl_httppost struct cannot be allocated
* SVNHTTP_FORMADD_MEMORY         if some allocation for string copying failed.
* SVNHTTP_FORMADD_ILLEGAL_ARRAY  if an illegal option is used in an array
*
***************************************************************************/
typedef enum {
	SVNHTTP_FORMADD_OK, /* first, no error */

	SVNHTTP_FORMADD_MEMORY,
	SVNHTTP_FORMADD_OPTION_TWICE,
	SVNHTTP_FORMADD_NULL,
	SVNHTTP_FORMADD_UNKNOWN_OPTION,
	SVNHTTP_FORMADD_INCOMPLETE,
	SVNHTTP_FORMADD_ILLEGAL_ARRAY,
	SVNHTTP_FORMADD_DISABLED, /* libcurl was built with this disabled */

	SVNHTTP_FORMADD_LAST /* last */
} SVPHTTPFORMcode;

CURL_EXTERN SVPHTTPFORMcode svn_http_formadd(struct svn_httppost **httppost, struct svn_httppost **last_post, ...);

CURL_EXTERN void svn_http_formfree(struct svn_httppost *form);

/* BEGIN: Modified by zhaixianqi 90006553, for 添加httpform的定义 2013/4/17   问题单号:新需求*/
typedef enum {
    SVN_HTTP_FORM_NOTHING,
    SVN_HTTP_FORM_COPYNAME,
    SVN_HTTP_FORM_PTRNAME,
    SVN_HTTP_FORM_NAMELENGTH,
    SVN_HTTP_FORM_COPYCONTENTS,
    SVN_HTTP_FORM_PTRCONTENTS,
    SVN_HTTP_FORM_CONTENTSLENGTH,
    SVN_HTTP_FORM_FILECONTENT,
    SVN_HTTP_FORM_ARRAY,
    SVN_HTTP_FORM_OBSOLETE,
    SVN_HTTP_FORM_FILE,
    SVN_HTTP_FORM_BUFFER,
    SVN_HTTP_FORM_BUFFERPTR,
    SVN_HTTP_FORM_BUFFERLENGTH,
    SVN_HTTP_FORM_CONTENTTYPE,
    SVN_HTTP_FORM_CONTENTHEADER,
    SVN_HTTP_FORM_FILENAME,
    SVN_HTTP_FORM_END,
    SVN_HTTP_FORM_OBSOLETE2,
    SVN_HTTP_FORM_STREAM,
    SVN_HTTP_FORM_LASTENTRY
    
}SVN_HTTP_FORMOPTION_E;
/* END:   Modified by zhaixianqi 90006553, 2013/4/17 */


#ifdef	__cplusplus
}
#endif

#endif
