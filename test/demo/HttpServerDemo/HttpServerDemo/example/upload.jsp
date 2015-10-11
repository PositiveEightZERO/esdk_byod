<%@ page contentType="text/html; charset=UTF-8" %>
<%@ taglib prefix="s" uri="/struts-tags" %>
<html>
    <head>
        <title>文件上传</title>
        <meta http-equiv="pragma" content="no-cache">
        <meta http-equiv="cache-control" content="no-cache">
        <meta http-equiv="expires" content="0">
    </head>
    <body>
        <form action="execute_upload.do" 
              enctype="multipart/form-data" method="post">
            文件:<input type="file" name="image">
                <input type="submit" value="上传" />
        </form>
        <br/>
    </body>
</html>