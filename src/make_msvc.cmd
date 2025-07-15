:: specify the path to the folder where MSVC is installed
call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"  
set "SOURCE_FILE=fcmp.c"
set "OUTPUT_FILE=fcmp.exe"
cl /EHsc "%SOURCE_FILE%" /Fe"%OUTPUT_FILE%"