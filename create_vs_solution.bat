@echo off
setlocal

echo Setup VS2022
PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer;%PATH%

for /f "usebackq delims=" %%i in (`vswhere.exe -latest -version "[17.7,17.9]" -requires Microsoft.Component.MSBuild -property installationPath`) do (
  set InstallDir=%%i
  if not exist "%%i\MSBuild\Current\Bin\MSBuild.exe" (
    echo error: Cannot locate Visual Studio 2019.
    goto error
  )
)

set __VCVARS=%InstallDir%\VC\Auxiliary\Build
call "%__VCVARS%\vcvarsall.bat" amd64 10.0.19041.0 -vcvars_ver=14.3

echo Fetching vcpkg
git submodule update --init --recursive

echo Building vcpkg
cd .\third_party\vcpkg

call .\bootstrap-vcpkg.bat
if %ERRORLEVEL% neq 0 goto vcpkg_error

echo Adding thirs party dependencies
vcpkg new --application
vcpkg add port gtest
vcpkg install --triplet=x64-windows-static


cd ..
cd ..

echo Cleanup build folder
rm -r build
mkdir build

echo Creating CMake files
cd build
cmake --version
cmake -DCMAKE_TOOLCHAIN_FILE=../third_party/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows-static -G "Visual Studio 17" -A x64 ..
if %ERRORLEVEL% neq 0 goto error
goto done

:vcpkg_error
echo Error installing vcpkg -- try manually updating repo from third_party/vcpkg
exit /b 1
goto done

:error
echo Project creation failure.
exit /b 1
goto done

:done
echo VS20220 solution file is ready in at build/ folder
