# esdk_byod
编译指南 （Android）

1、安装 Java，ant 添加环境变量JAVA_HOME, ANT_HOME

2、安装android sdk, 添加环境变量ANDROID_SDK_HOME

3、安装android ndk, 添加环境变量NDK_HOME

4、将%ANT_HOME%\bin;%NDK_HOME%;%ANDROID_SDK_HOME%/tools;%ANDROID_SDK_HOME%/platform-tools; 这些 路径加到path环境变量里

5、最后就可以在CI\script路径下运行ant -f build.xml开始编译了
