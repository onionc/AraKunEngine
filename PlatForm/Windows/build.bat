call VsDevCmd.bat

cl /EHsc /wd 4819 user32.lib gdi32.lib helloEngineWin.cpp
call helloEngineWin.exe
cmd /k
