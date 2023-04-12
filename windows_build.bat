ECHO OFF
::Cd into workspace folder
cd %~f1

::Clean old files
ECHO Clearing Old Build Files
del /f /q^
 build\*

::Compile
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
 ::Copy Binaries
 ECHO Copy Data
 copy lib\SDL\Windows\bin\SDL2.dll build\SDL2.dll

:end