echo off
pushd ..\
call vendor\bin\premake5\premake5.exe gmake
popd
PAUSE