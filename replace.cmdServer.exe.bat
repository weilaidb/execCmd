@echo on
taskkill /im cmdServer.exe /f 
copy /Y cmdServer-build-desktop-Qt_4_8_1_for_Desktop_-_MinGW__Qt_SDK____\release\cmdServer.exe D:\Software\Dropbox\yilinsendmail\cmdServer.exe
@echo copy done!!
explorer D:\Software\Dropbox\yilinsendmail\cmdServer.exe
pause