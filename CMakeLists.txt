cmake_minimum_required(VERSION 2.8)

option(PANDORA "Set to ON if targeting an OpenPandora device" ${PANDORA})
option(ODROID "Set to ON if targeting an ODroid device" ${ODROID})
option(PYRA "Set to ON if targeting an DragonBox Pyra device" ${PANDORA})
option(CHIP "Set to ON if targeting an PocketCHIP device" ${PANDORA})


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
#create a _SDL2 later?
add_definitions(-D_SDL)

find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)

INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${SDL_MIXER_INCLUDE_DIRS})

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
set(HCL_SDL_FILES
	src/sdl/audio.c
	src/sdl/input.c
	src/sdl/graphics.c
	src/sdl/system.c
	src/xBRZ/xbrz.cpp
	src/sdl/scale.cpp
)

if(PANDORA)
	add_definitions(-mcpu=cortex-a8)
	add_definitions(-mfpu=neon)
	add_definitions(-mfloat-abi=softfp)
	add_definitions(-ftree-vectorize)
endif(PANDORA)

set (EXTRA_LIBS ${EXTRA_LIBS} ${SDL_LIBRARY})
set (EXTRA_LIBS ${EXTRA_LIBS} ${SDL_MIXER_LIBRARY})

add_executable(hcl ${HCL_SOURCE_FILES} ${HCL_SDL_FILES})
target_link_libraries (hcl ${EXTRA_LIBS} m)
