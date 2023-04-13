ECHO OFF
::Cd into workspace folder
cd %~f1

::Check For Code Only Build
if "%~2"=="-c" goto :compile

::Clean old files
ECHO Clearing Old Build Files
rmdir /s /q build
mkdir build

::Compile
:compile
ECHO Compiling Project
g++^
 -std=c++17^
 -Wall^
 -Ilib\SDL\Windows\include^
 -Ilib\Lua\Windows\include^
 -Ilib\^
 -Ilib\sol^
 -Llib\SDL\Windows\lib^
 -Llib\Lua\Windows^
 -o build\game.exe^
 src\*.cpp^
 -lmingw32^
 -lSDL2main^
 -lSDL2^
 -lSDL2_image^
 -lSDL2_ttf^
 -lSDL2_mixer^
 -llua54^
 -g
::Last Arg is DEBUG FLAG

if errorlevel 0 goto :postbuild

ECHO Build Failed
goto:end

:postbuild
 ECHO Build Succeeded

::Check For Code Only Build
if "%~2"=="-c" goto :end

 ::Copy Binaries
 ECHO Copy Data
 xcopy /q lib\SDL\Windows\bin\*.dll build\
 xcopy /q lib\Lua\Windows\*.dll build\

 mkdir build\assets
 xcopy /q /e /h assets build\assets 

:end