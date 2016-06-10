set VCTargetsPath=c:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120

msbuild ..\Sigma.sln /v:n /p:Configuration=Core_lib
msbuild ..\Sigma.sln /v:n /p:Configuration=GUI_lib
msbuild ..\Sigma.sln /v:n /p:Configuration=UI_EXE
msbuild ..\Sigma.sln /v:n /p:Configuration=tests
