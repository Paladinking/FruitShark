@echo off
@setlocal enabledelayedexpansion

set SUCCES=TRUE

if [%1] EQU [] (
	goto :none
)

echo %1| findstr /i /l /x mingw >nul
if %errorlevel% EQU 0 (
	goto :mingw
)
echo %1| findstr /i /l /x gcc >nul
if %errorlevel% EQU 0 (
	goto :mingw
)
echo %1| findstr /i /x msvc >nul
if %errorlevel% EQU 0 (
	goto :msvc
)
echo %1| findstr /i /x cl >nul
if %errorlevel% EQU 0 (
	goto msvc
)
echo %1| findstr /i /x ninja >nul
if %errorlevel% EQU 0 (
	goto ninja
)
:none
echo invalid generator '%1'
exit /B

:ninja
set CMAKE_GEN=-G Ninja
set LIBS=libs-minGW
echo Generator: Ninja
goto :start
:mingw
set CMAKE_GEN=-G ^"MinGW Makefiles^" 
set LIBS=libs-minGW
echo Generator: minGW Makefiles
goto :start
:msvc
set CMAKE_GEN=
set LIBS=libs-msvc
echo Generator: msvc
goto :start

:getdir
echo Downloading %1
python -c "from urllib.request import urlretrieve as get ; get('%1', filename='temp.zip')"
python -m zipfile -e temp.zip .
del temp.zip
echo Done
exit /B

:failed
set SUCCES=FALSE
echo Failed building %1
exit /B

:start
if not exist %LIBS% (
	mkdir %LIBS%
)

:SDL2
cd %~dp0\%LIBS%
if not exist SDL2 (
	echo Building SDL2
	call :getdir https://github.com/libsdl-org/SDL/archive/refs/tags/release-2.26.5.zip
	mkdir SDL-release-2.26.5\build ^
	&& cd SDL-release-2.26.5\build ^
	&& cmake .. %CMAKE_GEN% -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%~dp0\%LIBS%\SDL2 ^
	&& cmake --build . --config Release ^
	&& cmake --install . --config Release || call :failed SDL2 && goto :SDL2_ttf
)
:SDL2_ttf
cd %~dp0\%LIBS%
rmdir SDL-release-2.26.5 /S /Q 2>nul
if not exist SDL2_ttf (
	echo Building SDL2_ttf
	call :getdir https://github.com/libsdl-org/SDL_ttf/archive/refs/tags/release-2.20.2.zip
	cd SDL_ttf-release-2.20.2
	call :getdir https://github.com/libsdl-org/freetype/archive/refs/tags/VER-2-13-0.zip
	rmdir external\freetype
	move freetype-VER-2-13-0 external\freetype || call :failed SDL2_ttf && goto :SDL2_image
	call :getdir https://github.com/libsdl-org/harfbuzz/archive/43931e3e596c04044861770b831c8f9452e2d3b0.zip
	rmdir external\harfbuzz
	move harfbuzz-43931e3e596c04044861770b831c8f9452e2d3b0 external\harfbuzz ^
	&& mkdir build ^
	&& cd build ^
	&& cmake .. %CMAKE_GEN% -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%~dp0\%LIBS%\SDL2_ttf -DCMAKE_PREFIX_PATH=%~dp0\%LIBS% -DSDL2TTF_HARFBUZZ=ON -DSDL2TTF_VENDORED=TRUE ^
	&& cmake --build . --config Release ^
	&& cmake --install . --config Release || call :failed SDL2_ttf && goto :SDL2_image
)
:SDL2_image
cd %~dp0\%LIBS%
rmdir SDL_ttf-release-2.20.2 /S /Q 2>nul
if not exist SDL2_image (
	echo Building SDL2_image
	call :getdir https://github.com/libsdl-org/SDL_image/archive/refs/tags/release-2.6.3.zip
	cd SDL_image-release-2.6.3 ^
	&& mkdir build ^
	&& cd build ^
	&& cmake .. %CMAKE_GEN% -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%~dp0\%LIBS%\SDL2_image -DCMAKE_PREFIX_PATH=%~dp0\%LIBS% -DSDL2IMAGE_VENDORED=TRUE ^
	&& cmake --build . --config Release ^
	&& cmake --install . --config Release || call :failed SDL2_image && goto :SDL2_mixer
)
:SDL2_mixer
cd %~dp0\%LIBS%
rmdir SDL_image-release-2.6.3 /S /Q 2>nul
if not exist SDL2_mixer (
	echo Building SDL2_mixer
	call :getdir https://github.com/libsdl-org/SDL_mixer/archive/refs/tags/release-2.6.3.zip
	cd SDL_mixer-release-2.6.3 || call :failed SDL2_mixer && goto :enet
	call :getdir https://github.com/libsdl-org/ogg/archive/790939cacc9a571bd2d3ca6c8fd49ddce5435399.zip
	rmdir external\ogg
	move ogg-790939cacc9a571bd2d3ca6c8fd49ddce5435399 external\ogg ^
	&& mkdir build && cd build ^
	&& cmake .. %CMAKE_GEN% -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%~dp0\%LIBS%\SDL2_mixer -DCMAKE_PREFIX_PATH=%~dp0\%LIBS% -DSDL2MIXER_OPUS=OFF -DSDL2MIXER_MOD=OFF -DSDL2MIXER_FLAC=OFF -DSDL2MIXER_VENDORED=TRUE ^
	&& cmake --build . --config Release ^
	&& cmake --install . --config Release || call :failed SDL2_mixer && goto :enet
)
:enet
cd %~dp0\%LIBS%
rmdir SDL_mixer-release-2.6.3 /S /Q 2>nul
if not exist enet (
	echo Building enet
	call :getdir https://github.com/lsalzman/enet/archive/refs/tags/v1.3.17.zip
	mkdir enet-1.3.17\build ^
	&& cd enet-1.3.17\build ^
	&& cmake .. %CMAKE_GEN% ^
	&& cmake --build . --config Release ^
	&& mkdir ..\..\enet || call :failed enet && goto :zlib
	if exist libenet.a (
		copy libenet.a ..\..\enet
	)
	if exist Release (
		copy Release\* ..\..\enet
	)
	move ..\include ..\..\enet\include
)
:zlib
cd %~dp0\%LIBS%
rmdir enet-1.3.17 /S /Q 2>nul
if not exist zlib (
	echo Building zlib
	call :getdir https://www.zlib.net/zlib1213.zip
	cd zlib-1.2.13 ^
	&& mkdir build ^
	&& cd build ^
	&& cmake .. %CMAKE_GEN% -DCMAKE_INSTALL_PREFIX=%~dp0\%LIBS%\zlib ^
	&& cmake --build . --config Release ^
	&& cmake --install . --config Release || call :failed zlib && goto :final
)
:final
cd %~dp0\%LIBS%
rmdir zlib-1.2.13 /S /Q 2>nul
cd %~dp0
if [%SUCCES%] NEQ [TRUE] (
	echo Build failed
	exit /B
) 

if not exist build (
	mkdir build
)
cd build
cmake .. %CMAKE_GEN% -DCMAKE_INSTALL_PREFIX=%~dp0\install -DLOCAL_LIBS=TRUE
cmake --build . --config Release
cmake --install . --config Release
cd ..

echo Done
exit /B