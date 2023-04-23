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
ECHO Starting Build
ECHO Comiling Source
cd build

::Compile Cpp files
for /r ..\src %%s in (*.cpp) do (
 g++ -c -std=c++17 -Wall -I..\lib\SDL\Windows\include -I..\lib\Lua\Windows\include -I..\lib\ -I..\lib\sol %%s -g
)

cd ../

::Linking Step
ECHO Linking Compiled Code
g++^
 -std=c++17^
 -Wall^
 -Llib\SDL\Windows\lib^
 -Llib\Lua\Windows^
 -o build\game.exe^
 build\*.o^
 -lmingw32^
 -lSDL2main^
 -lSDL2^
 -lSDL2_image^
 -lSDL2_ttf^
 -lSDL2_mixer^
 -llua54^
 -g
::Last Arg is DEBUG FLAG "-g"

del build\*.o

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