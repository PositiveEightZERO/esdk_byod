/** \file exml.h
 *  EXML: The Embedded XML parser, interface
 *  This header has all the information users need to know.
 *
 *	Features and limits
 *	> Fast and memory effective XML parser.
 *  > A simple way (a lite version of XPath) to access the XML nodes.
 *  > Traditional way to navigate: parent, child and sibling.
 *  > Tokens is stored linearly, use integer types to represent tokens.
 *  > Logical size limit of an XML file: 2G bytes.
 *  > Max token (element, text, attribute, etc.) length: 1M bytes,
 *    will truncate if oversized.
 *  > Max nested depth: 256, error if exceeded.
 *  > On demand token skipping, on demand text trimming using callbacks.
 *  > Encoding: ASCII, GB2312, UTF-8
 *  > Ignores comments, CDATA, DOCTYPE, etc.
 *  > The whole XML string needs to be persistent in memory.
 *  > The parser will modify the XML string, so not suitable for ROM XML.
 *  > Only works for 32-bit or 64-bit system.
 *  > Memory needed: normally 1.4X of XML string size, 3X at most.
 *
 *
 *  EXML uses tokens to store structurized information of an XML string in memory.
 *  That is:
 *  XML string "<root><Jack sex="male">text</Jack><Emma/></root>" is stored:
 *  TOKEN #   TYPE         DEPTH  STRING
 *      0   Element Open     0    "root"
 *      1   Element Open     1    "Jack"
 *      2   Attribute        1    "sex"
 *      3   Attribute Value  1    "male"
 *      4   Text             1    "text"
 *      5   Element Close    1    "Jack"
 *      6   Element Open     1    "Emma"
 *      7   Element Close    1    "Emma"
 *      8   Element Close    0    "root"
 *
 *  So, tokens can be represented by integers (TOKEN #).
 *  In EXML, there are 3 important data types: "XMLHanlde", "XMLCursor" and "int".
 *    Use type XMLHandle to store EXML parser objects;
 *    Use type XMLCursor to store EXML cursor objects;
 *    Use type int to store token handles, EOF indicates an invalid token.
 *
 *  A Typical Usage:
 *    XMLHandle xml; // the EXML object
 *    XMLCursor cursor; // the cursor
 *    int i; // the element (one type of token)
 *    xml = EXML_createbyfile("file.xml");  // or EXML_createbystr("<root>hello</root>");
 *    EXML_parse(xml);
 *    EXML_get_element(xml, 0, "/root/foo[1]");  // access one element
 *    cursor = EXML_open_cursor(xml, 0, "/root/bar[@lang]"); // access multiple elements
 *    for (i = EXML_cursor_first(cursor); i != EOF; i = EXML_cursor_next(cursor))
 *        EXML_get_text(xml, i); // or EXML_get_attr(); EXML_get_name(); EXML_elem_text();
 *    EXML_close_cursor(cursor);
 *    EXML_close(xml);
 *
 *  A Lite Version of XPath:
 *                      / : start from the root
 *                     ./ : start from current element
 *           /root/foo[2] : the second occorence of element "foo"
 *        /root/foo[@bar] : element "foo" that has attribute "bar"
 *        ./foo[@bar=baz] : element "foo" that has an attirubte "bar" with value "baz"
 *        ./.[3!@bar/]foo : element "foo" under the 3rd occurence of any element not having an attribute named "bar"
 *           ./foo[!@bar] : not having attribute named "bar"
 *       ./foo[!@bar=baz] : not having attribute named "bar" having value "baz", like NOT(@bar=baz)
 *
 *
 *    Wu Hao (howe@huaweisymantec.com)
 *    Huawei Symantec
 */

#ifndef _ANYOFFICE_EXML_H_
#define _ANYOFFICE_EXML_H_
#ifdef  __cplusplus
extern "C" {
#endif

/** XML token types */
typedef enum tagTokenType
{
    TOKEN_OPEN = 0,
    TOKEN_CLOSE,
    TOKEN_TEXT,
    TOKEN_ATTR,
    TOKEN_ATTR_VAL,
    TOKEN_PI,
    TOKEN_PI_VAL
} TokenType;


/** Encoding types */
typedef enum tagEncoding
{
    ENCODING_ANSI = 0,
    ENCODING_GBK,   /* including GB2312 */
    ENCODING_UTF8,
    ENCODING_NOT_SUPPORT
} Encoding;

/** Handle to the EXML parser object */
typedef void * XMLHandle;
/** Cursor is used to access multiple tokens in an EXML object. */
typedef void * XMLCursor;

/** BOOL type for EXML functions */
typedef int EXML_BOOL;
#define EXML_TRUE (-1)
#define EXML_FALSE (0)

/** Callbacks for custom token skipping.
 * \return EXML_TRUE if need so, return EXML_FALSE if not.
 */
typedef EXML_BOOL (*EXMLSkipCB)(TokenType t, int depth, const char *text, int length);

/** Callbacks for custom token text trimming.
 * \return EXML_TRUE if need so, return EXML_FALSE if not.
 */
typedef EXML_BOOL (*EXMLTrimCB)(TokenType t, int depth, const char *text, int length);

/** Given an XML string, creates an EXML object and returns a handle to it.
 *  \param xmlstr XML string.
 *  \param length Length of the XML string in bytes.
 *  \return Handle to the EXML object, NULL if fails.
 */
extern XMLHandle EXML_createbystr(char *xmlstr, unsigned long length);

/** Given a file name, creates an EXML object and returns a handle to it.
 *  \param xmlfile Filename.
 *  \return Handle to the EXML object, NULL if fails.
 */
extern XMLHandle EXML_createbyfile(const char *xmlfile);

/** Close the EXML object by its handle.
 *  \param hxml Handle to an EXML object.
 */
extern void EXML_close(XMLHandle hxml);

/** Set custom callbacks for skipping tokens or trimming text.
 *  \param hxml Handle to an EXML object.
 *  \param skip_cb A callback for custom skipping.
 *      Only works with TOKEN_OPEN, TOKEN_ATTR, TOKEN_PI.
 *  \param trim_cb A callback for custom text trimming.
 *      Only works with TOKEN_TEXT, TOKEN_ATTR_VALUE.
 */
extern void EXML_setcallback(XMLHandle hxml, EXMLSkipCB skip_cb, EXMLTrimCB trim_cb);

/** Parse the XML string.
 *  \param hxml Handle to an EXML object.
 *  \return EXML_TRUE if successes, EXML_FALSE if fails.
 */
extern EXML_BOOL EXML_parse(XMLHandle hxml);

/** Dump internal information of an EXML object.
 *  \param hxml Handle to an EXML object.
 */
extern void EXML_dump(XMLHandle hxml);

/** Get the root element.
 *  \param hxml Handle to an EXML object.
 *  \return The root element.
 */
extern int EXML_root(XMLHandle hxml);

/** Get the element by XML path, only the first will be selected.
 *  \param hxml Handle to an EXML object.
 *  \param pos Start position to search.
 *  \param xpath XML XPath-like expression.
 *  \return The element, EOF if not found.
 */
extern int EXML_get_element(XMLHandle hxml, int pos, const char *xpath);

/** \name Cursor operations
 *  \brief Cursor is designed for selecting and browsing through multiple elements.
 *  Typical procedures are:
 *  for (int i = EXML_cursor_first(cur); i != EOF; i = EXML_cursor_next(cur))
 *  { ... }
 */
/*@{*/
/** Open a cursor by XML path.
 *  You should always close a cursor by EXML_close_cursor.
 *  \param hxml Handle to an EXML object.
 *  \param pos Handle to an EXML object.
 *  \param xpath XPath-like expression.
 *  \return A handle to the cursor object, NULL if failed.
 */
extern XMLCursor EXML_open_cursor(XMLHandle hxml, int pos, const char *xpath);

/** Close a cursor.
 *  \param hxml Handle to an EXML object.
 */
extern void EXML_close_cursor(XMLCursor cur);

/** Get the first element from a cursor.
 *  \param cur Handle to a cursor object.
 *  \return The element, EOF if no element in the cursor.
 */
extern int EXML_cursor_first(XMLCursor cur);

/** Get the next element from a cursor.
 *  \param cur Handle to a cursor object.
 *  \return The element, EOF if no more element.
 */
extern int EXML_cursor_next(XMLCursor cur);
/*@}*/

/** Get the name of an element
 *  \param hxml Handle to an EXML object.
 *  \param element The element.
 *  \return Element name, NULL for invalid inputs.
 */
extern char *EXML_get_name(XMLHandle hxml, int element);

/** Get the text within an element
 *  \param hxml Handle to an EXML object.
 *  \param element The element.
 *  \return Text within the element, NULL for invalid inputs, "" for not found.
 */
extern  char *EXML_get_text(XMLHandle hxml, int element);

/** Get the value of an attribute
 *  \param hxml Handle to an EXML object.
 *  \param element The element.
 *  \pram attr The name of an attribute.
 *  \return The attribute value, NULL for not found or invalid inputs.
 */
extern const char *EXML_get_attr(XMLHandle hxml, int element, const char *attr);

/** Get the text within an element assigned by its path.
 *  \param hxml Handle to an EXML object.
 *  \param pos Start position to search.
 *  \param xpath XPath-like expression.
 *  \param ifnull Replacement of the return value if the element is not found.
 *  \return The text within an element assigned by its path, return ifnull if not found.
 */
extern const char *EXML_elem_text(XMLHandle hxml, int pos, const char *xpath, char *ifnull);

/** Get processing instruction token by given name.
 *  \param hxml Handle to an EXML object.
 *  \param pos Start position to search.
 *  \param pi pi name.
 *  \return The PI token id, EOF if not found.
 */
extern int EXML_get_pi(XMLHandle hxml, int pos, const char *pi);

/** Get processing instruction value by given name.
 *  \param hxml Handle to an EXML object.
 *  \param pi pi token id.
 *  \return The value of a pi, NULL if pi not valid, "" if no value found
 */
extern const char *EXML_pi_value(XMLHandle hxml, int pi);

/** \name Traditional navigating operations
 */
/*@{*/

/** Get parent element
 *  \param hxml Handle to an EXML object.
 *  \param element The reference element.
 *  \return The parent element of the reference, EOF if not found.
 */
extern int EXML_parent(XMLHandle hxml, int element);

/** Get the first child element
 *  \param hxml Handle to an EXML object.
 *  \param element The reference element.
 *  \return The first child element of the reference, EOF if not found.
 */
extern int EXML_firstchild(XMLHandle hxml, int element);

/** Get the last child element
 *  \param hxml Handle to an EXML object.
 *  \param element The reference element.
 *  \return The last child element of the reference, EOF if not found.
 */
extern int EXML_lastchild(XMLHandle hxml, int element);

/** Get next sibling element
 *  \param hxml Handle to an EXML object.
 *  \param element The reference element.
 *  \return The next sibling element of the reference, EOF if not found.
 */
extern int EXML_nextsibling(XMLHandle hxml, int element);

/** Get previous sibling element
 *  \param hxml Handle to an EXML object.
 *  \param element The reference element.
 *  \return The previous sibling element of the reference, EOF if not found.
 */
extern int EXML_prevsibling(XMLHandle hxml, int element);

/*@}*/


/** Replace predefined entities with real characters
 * \param dest Destination buffer storing target string, always terminated by '\0'.
 * \param size Destination buffer size.
 * \param src Source string that may contain predefined entities, '\0' terminated.
 * \return Number of characters copied into destination buffer, not including trailing '\0'.
 */
int EXML_parse_entity(char *dest, int size, const char *src);

/** Convert UTF8 string into EUC-CN (GB2312) string
 * \param dest Destination buffer storing target string, always terminated by '\0'.
 * \param size Destination buffer size.
 * \param src Source UTF8 string, '\0' terminated.
 * \return Number of characters copied into destination buffer, not including trailing '\0'.
 */
int EXML_uc2gb2312(unsigned char *dest, int size, const unsigned char *src);
#ifdef  __cplusplus
}
#endif

#endif /* _EXML_H_ */
