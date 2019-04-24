@echo on
taskkill /im cmdServer.exe /f 
taskkill /im execWinCmdServer2.exe /f 
copy /Y cmdServer-build-desktop-Qt_4_8_1_for_Desktop_-_MinGW__Qt_SDK____\release\cmdServer.exe D:\Dropbox\cmdServer_lib4.8.1_v6.1\
copy /Y execWinCmdServer2\release\execWinCmdServer2.exe  D:\Dropbox\cmdServer_lib4.8.1_v6.1\
@echo copy done!!
REM explorer D:\Software\Dropbox\yilinsendmail\cmdServer.exe
pause