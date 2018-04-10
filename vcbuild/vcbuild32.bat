@ECHO off

if not exist dist32 mkdir dist32
if exist dist32\* del /q dist32\*

cd ..\libndtypes
copy /y Makefile.vc Makefile
copy /y ndtypes.h.in ndtypes.h

cd compat
copy /y Makefile.vc Makefile
cd ..

cd serialize
copy /y Makefile.vc Makefile
cd ..

nmake /nologo clean
nmake /nologo

copy /y libndtypes-0.2.0dev3.lib ..\vcbuild\dist32
copy /y libndtypes-0.2.0dev3.dll ..\vcbuild\dist32
copy /y libndtypes-0.2.0dev3.dll.lib ..\vcbuild\dist32
copy /y libndtypes-0.2.0dev3.dll.exp ..\vcbuild\dist32
copy /y ndtypes.h ..\vcbuild\dist32

cd tests
copy /y Makefile.vc Makefile
nmake /nologo clean
nmake /nologo

copy /y runtest.exe ..\..\vcbuild\dist32
copy /y runtest_shared.exe ..\..\vcbuild\dist32

cd ..\..\vcbuild



