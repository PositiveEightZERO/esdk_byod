SET BUILD_ID=BYOD_FORTIFY
rem ������jar��
SET CLASSPATH=%ANDROID_SDK_HOME%\platforms\android-19\android.jar
rem ɨ��Ĺ���Ŀ¼
SET SCAN_DIR=..\..\test\demo\CordovaTestApp\platforms\android\
sourceanalyzer -b %BUILD_ID% -clean

sourceanalyzer -b %BUILD_ID% -machine-output -cp %CLASSPATH% -source 1.7 %SCAN_DIR%\**\InAppBrowser.java -Xmx1024m
