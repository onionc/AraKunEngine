call VsDevCmd.bat
;; -wd 4819 去除警告：warning C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失
cl -wd4819 user32.lib gdi32.lib helloEngineWin.cpp
cmd /k