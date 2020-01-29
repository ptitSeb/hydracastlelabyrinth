cmake_minimum_required(VERSION 2.8)

option(BITTBOY "Set to ON if targeting a Bittboy device" ${BITTBOY})
option(GAMESHELL "Set to ON if targeting a GameShell device" ${GAMESHELL})
option(PANDORA "Set to ON if targeting an OpenPandora device" ${PANDORA})
option(ODROID "Set to ON if targeting an ODroid device" ${ODROID})
option(PYRA "Set to ON if targeting a DragonBox Pyra device" ${PYRA})
option(CHIP "Set to ON if targeting a PocketCHIP device" ${CHIP})
option(USE_CCACHE "Set to ON to use ccache if present in the system" ${USE_CCACHE})
option(EMSCRIPTEN "Set to ON to build using EMSCRIPTEN" ${EMSCRIPTEN})

option(USE_SDL2 "Use SDL2 instead of SDL1.2" ${USE_SDL2})

if(CMAKE_C_COMPILER EQUAL "emcc")
	set(EMSCRIPTEN ON)
endif()

if(EMSCRIPTEN)
	add_definitions(-D_SDL)
	add_definitions(-D_SDL2)
	add_definitions(-DEMSCRIPTEN)
	add_definitions("-s" "USE_SDL=2")
	add_definitions("-s" "USE_SDL_MIXER=2")
	add_definitions("-s" "USE_OGG=1")
	add_definitions("-s" "USE_VORBIS=1")
	add_definitions("-s" "ASSERTIONS=1")
	add_compile_options("-emit-llvm")
	add_compile_options("-std=c++11")
	add_definitions("-s" "WASM=1")
	add_definitions("--preload-file" "data")
	#should force all def to false
	if(NOT CMAKE_C_COMPILER EQUAL "emcc")
		set(CMAKE_C_COMPILER emcc CACHE PATH "" FORCE)
		set(CMAKE_CXX_COMPILER emcc CACHE PATH "" FORCE)
		set(CMAKE_C_COMPILER_AR emar CACHE PATH "" FORCE)
		set(CMAKE_C_COMPILER_RANLIB emranlib CACHE PATH "" FORCE)
		#set(CMAKE_CROSSCOMPILING_EMULATOR node_js CACHE PATH "" FORCE)
	endif()
	if(NOT USE_SDL2)
		set(USE_SDL2 ON CACHE BOOL ON FORCE)
	endif()
	set(SDL2_LIBRARIES SDL2)
else(EMSCRIPTEN)

if(BITTBOY)
	add_definitions(-DBITTBOY)
endif()
if(GAMESHELL)
	add_definitions(-DGAMESHELL)
endif()
if(PANDORA)
	add_definitions(-DPANDORA)
endif()
if(PYRA)
	add_definitions(-DPYRA)
endif()
if(CHIP)
	add_definitions(-DCHIP)
endif()
if(ODROID)
	add_definitions(-DODROID)
endif()

if(USE_SDL2)
	add_definitions(-D_SDL -D_SDL2)
	find_package(SDL2 REQUIRED)
	#find_package(SDL2_mixer REQUIRED)
	
	INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
	#INCLUDE_DIRECTORIES(${SDL2_MIXER_INCLUDE_DIRS})
else()
	add_definitions(-D_SDL)
	find_package(SDL REQUIRED)
	find_package(SDL_mixer REQUIRED)
	
	INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIRS})
	INCLUDE_DIRECTORIES(${SDL_MIXER_INCLUDE_DIRS})
endif()
endif(EMSCRIPTEN)

if(USE_CCACHE)
    find_program(CCACHE_FOUND ccache)
    if(CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    endif()
endif()

#Need c++11 and c99
if (CMAKE_VERSION VERSION_LESS "3.1")
	if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
		set (CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")
		set (CMAKE_CXX_FLAGS "-std=c+11 ${CMAKE_CXX_FLAGS}")
	endif ()
else ()
	set (CMAKE_C_STANDARD 99)
	set (CMAKE_CXX_STANDARD 11)
endif ()

set(HCL_SOURCE_FILES
	src/collision.c
	src/effect.c
	src/enemy.c
	src/game.c
	src/hero.c
	src/ini.c
	src/inventory.c
	src/main.c
	src/object.c
	src/options.c
	src/PHL.c
	src/platform.c
	src/qda.c
	src/stagedata.c
	src/text.c
	src/titlescreen.c
	src/weapon.c
	src/enemies/batboss.c
	src/enemies/bat.c
	src/enemies/bee.c
	src/enemies/boar.c
	src/enemies/boomknight.c
	src/enemies/crab.c
	src/enemies/devil.c
	src/enemies/dodo.c
	src/enemies/dog.c
	src/enemies/firewheel.c
	src/enemies/fish.c
	src/enemies/garm.c
	src/enemies/gas.c
	src/enemies/ghoul.c
	src/enemies/golem.c
	src/enemies/gyra.c
	src/enemies/heads.c
	src/enemies/hydra.c
	src/enemies/jellyfish.c
	src/enemies/knight.c
	src/enemies/lolidra.c
	src/enemies/pendulum.c
	src/enemies/podoboo.c
	src/enemies/poisonknight.c
	src/enemies/pumpkin.c
	src/enemies/seal.c
	src/enemies/skeleton.c
	src/enemies/skull.c
	src/enemies/slime.c
	src/enemies/slug.c
	src/enemies/thwomp.c
	src/enemies/waterjumper.c
	src/enemies/wizard.c
)
if(USE_SDL2)
	set(HCL_SDL_FILES
		src/sdl2/audio.c
		src/sdl2/input.c
		src/sdl2/graphics.c
		src/sdl2/system.c
		src/xBRZ/xbrz.cpp
		src/sdl2/scale.cpp
	)
else()
	set(HCL_SDL_FILES
		src/sdl/audio.c
		src/sdl/input.c
		src/sdl/graphics.c
		src/sdl/system.c
		src/xBRZ/xbrz.cpp
		src/sdl/scale.cpp
	)
endif()

if(PANDORA)
	add_definitions(-mcpu=cortex-a8)
	add_definitions(-mfpu=neon)
	add_definitions(-mfloat-abi=softfp)
	add_definitions(-ftree-vectorize)
endif(PANDORA)

if(NOT EMSCRIPTEN)
	if(USE_SDL2)
		set (EXTRA_LIBS ${EXTRA_LIBS} ${SDL2_LIBRARIES})
		set (EXTRA_LIBS ${EXTRA_LIBS} SDL2_mixer)
	else()
		set (EXTRA_LIBS ${EXTRA_LIBS} ${SDL_LIBRARY})
		set (EXTRA_LIBS ${EXTRA_LIBS} ${SDL_MIXER_LIBRARY})
	endif()
endif()

add_executable(hcl ${HCL_SOURCE_FILES} ${HCL_SDL_FILES})
target_link_libraries (hcl ${EXTRA_LIBS} m)
if(EMSCRIPTEN)
	#set(CMAKE_EXECUTABLE_SUFFIX ".html")
	target_compile_options(hcl PUBLIC "-O2")
	add_custom_command(TARGET hcl PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:hcl>/data)
	#target_compile_options(hcl "-O2")
	#target_link_options(hcl PRIVATE USE_SDL=2) #needs cmake 3.13+
	SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s USE_SDL=2 -s USE_SDL_MIXER=2 -s USE_OGG=1 -s USE_VORBIS=1 -s WASM=1")
	SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} --emrun --preload-file data -s ASSERTIONS=1 -s TOTAL_MEMORY=128MB -s ALLOW_MEMORY_GROWTH=1 -s EXIT_RUNTIME=1")
	SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} --shell-file ${CMAKE_SOURCE_DIR}/src/template.html")
	#target_link_libraries(hcl "--preload-file" "data")
	set_target_properties(hcl PROPERTIES OUTPUT_NAME "${CMAKE_SOURCE_DIR}/docs/index.html")
endif(EMSCRIPTEN)
