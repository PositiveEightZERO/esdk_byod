/* Лицензионное соглашение на использование набора средств разработки
 * «SDK Яндекс.Диска» доступно по адресу: http://legal.yandex.ru/sdk_agreement
 */


#ifdef __cplusplus
#   define SVN_EXTERN  extern "C" __attribute__((visibility ("default")))
#else
#   define SVN_EXTERN  extern __attribute__((visibility ("default")))
#endif

#define Svn_DEBUG YES

#ifdef Svn_DEBUG
#define SvnLog(...) NSLog(__VA_ARGS__)
#else
#define SvnLog(...) {}
#endif


SVN_EXTERN NSInteger const kSVNErrorUnknown;

SVN_EXTERN NSString *const kSVNBadArgumentErrorDomain;
SVN_EXTERN NSString *const kSVNBadResponseErrorDomain;
SVN_EXTERN NSString *const kSVNConnectionErrorDomain;
SVN_EXTERN NSString *const kSVNRequestErrorDomain;
//

SVN_EXTERN NSString *const kSVNResponseError;


#undef SVN_EXTERN