@echo off
@setlocal enabledelayedexpansion

if [%1] EQU [MinGW] (
	set CMAKE_GEN=-G ^"MinGW Makefiles^"
	set LIBS=libs-minGW
) else (
	set CMAKE_GEN=
	set LIBS=libs-msvc
)


if not exist %LIBS% (
	mkdir %LIBS%
)


cd %LIBS%
if not exist SDL2 (
	powershell -c "wget https://github.com/libsdl-org/SDL/archive/refs/tags/release-2.26.5.zip -o SDL.zip"
	python -c "import zipfile; file = zipfile.ZipFile('SDL.zip', 'r') ; file.extractall('.') ; file.close()"
	del SDL.zip
	cd SDL-release-2.26.5
	mkdir build
	cd build
	cmake .. %CMAKE_GEN% -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%~dp0\%LIBS%\SDL2
	cmake --build . --config Release
	cmake --install . --config Release
	cd ..\..
	rmdir SDL-release-2.26.5 /S /Q
)
if not exist SDL2_ttf (
	powershell -c "wget https://github.com/libsdl-org/SDL_ttf/archive/refs/tags/release-2.20.2.zip -o SDL_ttf.zip"
	python -c "import zipfile; file = zipfile.ZipFile('SDL_ttf.zip', 'r') ; file.extractall('.') ; file.close()"
	del SDL_ttf.zip
	cd SDL_ttf-release-2.20.2
	powershell -c "wget https://github.com/libsdl-org/freetype/archive/refs/tags/VER-2-13-0.zip -o freetype.zip"
	python -c "import zipfile; file = zipfile.ZipFile('freetype.zip', 'r') ; file.extractall('.') ; file.close()"
	rmdir external\freetype
	move freetype-VER-2-13-0 external\freetype
	del freetype.zip
	powershell -c "wget https://github.com/libsdl-org/harfbuzz/archive/43931e3e596c04044861770b831c8f9452e2d3b0.zip -o harfbuzz.zip"
	python -c "import zipfile; file = zipfile.ZipFile('harfbuzz.zip', 'r') ; file.extractall('.') ; file.close()"
	rmdir external\harfbuzz
	move harfbuzz-43931e3e596c04044861770b831c8f9452e2d3b0 external\harfbuzz
	del harfbuzz.zip
	mkdir build
	cd build
	cmake .. %CMAKE_GEN% -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%~dp0\%LIBS%\SDL2_ttf -DCMAKE_PREFIX_PATH=%~dp0\%LIBS% -DSDL2TTF_HARFBUZZ=ON -DSDL2TTF_VENDORED=TRUE
	cmake --build . --config Release
	cmake --install . --config Release
	cd ..\..
	rmdir SDL_ttf-release-2.20.2 /S /Q
)
if not exist SDL2_image (
	powershell -c "wget https://github.com/libsdl-org/SDL_image/archive/refs/tags/release-2.6.3.zip -o SDL_image.zip"
	python -c "import zipfile; file = zipfile.ZipFile('SDL_image.zip', 'r') ; file.extractall('.') ; file.close()"
	del SDL_image.zip
	cd SDL_image-release-2.6.3
	mkdir build
	cd build
	cmake .. %CMAKE_GEN% -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%~dp0\%LIBS%\SDL2_image -DCMAKE_PREFIX_PATH=%~dp0\%LIBS% -DSDL2IMAGE_VENDORED=TRUE
	cmake --build . --config Release
	cmake --install . --config Release
	cd ..\..
	rmdir SDL_image-release-2.6.3 /S /Q
)

if not exist SDL2_mixer (
	powershell -c "wget https://github.com/libsdl-org/SDL_mixer/archive/refs/tags/release-2.6.3.zip -o SDL_mixer.zip"
	python -c "import zipfile; file = zipfile.ZipFile('SDL_mixer.zip', 'r') ; file.extractall('.') ; file.close()"
	del SDL_mixer.zip
	cd SDL_mixer-release-2.6.3
	powershell -c "wget https://github.com/libsdl-org/ogg/archive/790939cacc9a571bd2d3ca6c8fd49ddce5435399.zip -o ogg.zip"
	python -c "import zipfile; file = zipfile.ZipFile('ogg.zip', 'r') ; file.extractall('.') ; file.close()"
	rmdir external\ogg
	move ogg-790939cacc9a571bd2d3ca6c8fd49ddce5435399 external\ogg
	del ogg.zip
	mkdir build
	cd build
	cmake .. %CMAKE_GEN% -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%~dp0\%LIBS%\SDL2_mixer -DCMAKE_PREFIX_PATH=%~dp0\%LIBS% -DSDL2MIXER_OPUS=OFF -DSDL2MIXER_MOD=OFF -DSDL2MIXER_FLAC=OFF -DSDL2MIXER_VENDORED=TRUE -DWITH_STACK_PROTECTOR=OFF
	cmake --build . --config Release
	cmake --install . --config Release
	cd ..\..
	rmdir SDL_mixer-release-2.6.3 /S /Q
)

if not exist enet (
	powershell -c "wget https://github.com/lsalzman/enet/archive/refs/tags/v1.3.17.zip -o enet.zip"
	python -c "import zipfile; file = zipfile.ZipFile('enet.zip', 'r') ; file.extractall('.') ; file.close()"
	del enet.zip
	cd enet-1.3.17
	mkdir build
	cd build
	cmake .. %CMAKE_GEN%
	cmake --build . --config Release
	mkdir ..\..\enet
	if exist libenet.a (
		copy libenet.a ..\..\enet
	)
	if exist Release (
		copy Release\* ..\..\enet
	)
	move ..\include ..\..\enet\include
	cd ..\..
	rmdir enet-1.3.17 /S /Q
)

if not exist zlib (
	powershell -c "wget https://www.zlib.net/zlib1213.zip -o zlib.zip"
	python -c "import zipfile; file = zipfile.ZipFile('zlib.zip', 'r') ; file.extractall('.') ; file.close()"
	del zlib.zip
	cd zlib-1.2.13
	mkdir build
	cd build
	cmake .. %CMAKE_GEN% -DCMAKE_INSTALL_PREFIX=%~dp0\%LIBS%\zlib
	for /f "delims=" %%i in ('type CMakeCache.txt ^| findstr CMAKE_RC_COMPILER:FILEPATH=') do (
		set RC_COMPILER=%%i
	)
	call :RC_FIX "!RC_COMPILER:~27!"
	cd ..
	rmdir build /S /Q
	mkdir build
	cd build
	cmake .. %CMAKE_GEN% -DCMAKE_INSTALL_PREFIX=%~dp0\%LIBS%\zlib
	cmake --build . --config Release
	cmake --install . --config Release
	cd ..\..
	rmdir zlib-1.2.13 /S /Q
)
exit /B

:RC_FIX
	echo %1
	set RC_COMPILER=%~s1
	echo set(CMAKE_RC_COMPILER !RC_COMPILER:\=/!) >> ..\rc_fix_temp
	type ..\CMakeLists.txt >> ..\rc_fix_temp
	copy /Y ..\rc_fix_temp ..\CMakeLists.txt
	exit /b