SET BUILD_ID=BYOD_FORTIFY




sourceanalyzer -b %BUILD_ID% -clean

sourceanalyzer -b %BUILD_ID% -machine-output -cp %CLASSPATH% -source 1.7 %SCAN_DIR%\**\InAppBrowser.java -Xmx1024m