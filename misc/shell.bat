@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x64
doskey l=dir
doskey vs=devenv w:\build\win32_handmade.exe
set path=w:\handmade\misc;%path%