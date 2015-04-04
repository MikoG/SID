#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/SID_api.o \
	${OBJECTDIR}/SID_audio.o \
	${OBJECTDIR}/SID_biome.o \
	${OBJECTDIR}/SID_biome_paths.o \
	${OBJECTDIR}/SID_biomes.o \
	${OBJECTDIR}/SID_button.o \
	${OBJECTDIR}/SID_camera.o \
	${OBJECTDIR}/SID_character.o \
	${OBJECTDIR}/SID_character_strip.o \
	${OBJECTDIR}/SID_chunk.o \
	${OBJECTDIR}/SID_chunks.o \
	${OBJECTDIR}/SID_control.o \
	${OBJECTDIR}/SID_display.o \
	${OBJECTDIR}/SID_draw_control.o \
	${OBJECTDIR}/SID_draw_vbo.o \
	${OBJECTDIR}/SID_environs.o \
	${OBJECTDIR}/SID_euclid.o \
	${OBJECTDIR}/SID_file.o \
	${OBJECTDIR}/SID_font.o \
	${OBJECTDIR}/SID_fonts.o \
	${OBJECTDIR}/SID_forms.o \
	${OBJECTDIR}/SID_frustum.o \
	${OBJECTDIR}/SID_globals.o \
	${OBJECTDIR}/SID_height_map.o \
	${OBJECTDIR}/SID_interactive_object.o \
	${OBJECTDIR}/SID_keyboard.o \
	${OBJECTDIR}/SID_layout.o \
	${OBJECTDIR}/SID_menu_bar.o \
	${OBJECTDIR}/SID_mouse.o \
	${OBJECTDIR}/SID_object.o \
	${OBJECTDIR}/SID_object_ingame.o \
	${OBJECTDIR}/SID_object_template.o \
	${OBJECTDIR}/SID_object_templates.o \
	${OBJECTDIR}/SID_perlin.o \
	${OBJECTDIR}/SID_plot.o \
	${OBJECTDIR}/SID_plots.o \
	${OBJECTDIR}/SID_radio_button.o \
	${OBJECTDIR}/SID_rectangle_2d.o \
	${OBJECTDIR}/SID_rectangle_3d.o \
	${OBJECTDIR}/SID_regions.o \
	${OBJECTDIR}/SID_render.o \
	${OBJECTDIR}/SID_select_3drect.o \
	${OBJECTDIR}/SID_select_delegate.o \
	${OBJECTDIR}/SID_select_vbo.o \
	${OBJECTDIR}/SID_sidof.o \
	${OBJECTDIR}/SID_sidofs.o \
	${OBJECTDIR}/SID_structure.o \
	${OBJECTDIR}/SID_texture.o \
	${OBJECTDIR}/SID_textures.o \
	${OBJECTDIR}/SID_timer.o \
	${OBJECTDIR}/SID_toggle.o \
	${OBJECTDIR}/SID_user_events.o \
	${OBJECTDIR}/SID_vbo.o \
	${OBJECTDIR}/SID_vector3.o \
	${OBJECTDIR}/SID_vertex3.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m32
CXXFLAGS=-m32

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../SID/dist/Debug/GNU-Linux-x86 -lSID

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sid

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sid: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sid ${OBJECTFILES} ${LDLIBSOPTIONS} -lSDL2_image -lSDL2_mixer `sdl2-config --cflags --libs`

${OBJECTDIR}/SID_api.o: SID_api.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_api.o SID_api.cpp

${OBJECTDIR}/SID_audio.o: SID_audio.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_audio.o SID_audio.cpp

${OBJECTDIR}/SID_biome.o: SID_biome.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_biome.o SID_biome.cpp

${OBJECTDIR}/SID_biome_paths.o: SID_biome_paths.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_biome_paths.o SID_biome_paths.cpp

${OBJECTDIR}/SID_biomes.o: SID_biomes.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_biomes.o SID_biomes.cpp

${OBJECTDIR}/SID_button.o: SID_button.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_button.o SID_button.cpp

${OBJECTDIR}/SID_camera.o: SID_camera.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_camera.o SID_camera.cpp

${OBJECTDIR}/SID_character.o: SID_character.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_character.o SID_character.cpp

${OBJECTDIR}/SID_character_strip.o: SID_character_strip.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_character_strip.o SID_character_strip.cpp

${OBJECTDIR}/SID_chunk.o: SID_chunk.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_chunk.o SID_chunk.cpp

${OBJECTDIR}/SID_chunks.o: SID_chunks.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_chunks.o SID_chunks.cpp

${OBJECTDIR}/SID_control.o: SID_control.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_control.o SID_control.cpp

${OBJECTDIR}/SID_display.o: SID_display.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_display.o SID_display.cpp

${OBJECTDIR}/SID_draw_control.o: SID_draw_control.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_draw_control.o SID_draw_control.cpp

${OBJECTDIR}/SID_draw_vbo.o: SID_draw_vbo.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_draw_vbo.o SID_draw_vbo.cpp

${OBJECTDIR}/SID_environs.o: SID_environs.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_environs.o SID_environs.cpp

${OBJECTDIR}/SID_euclid.o: SID_euclid.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_euclid.o SID_euclid.cpp

${OBJECTDIR}/SID_file.o: SID_file.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_file.o SID_file.cpp

${OBJECTDIR}/SID_font.o: SID_font.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_font.o SID_font.cpp

${OBJECTDIR}/SID_fonts.o: SID_fonts.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_fonts.o SID_fonts.cpp

${OBJECTDIR}/SID_forms.o: SID_forms.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_forms.o SID_forms.cpp

${OBJECTDIR}/SID_frustum.o: SID_frustum.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_frustum.o SID_frustum.cpp

${OBJECTDIR}/SID_globals.o: SID_globals.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_globals.o SID_globals.cpp

${OBJECTDIR}/SID_height_map.o: SID_height_map.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_height_map.o SID_height_map.cpp

${OBJECTDIR}/SID_interactive_object.o: SID_interactive_object.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_interactive_object.o SID_interactive_object.cpp

${OBJECTDIR}/SID_keyboard.o: SID_keyboard.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_keyboard.o SID_keyboard.cpp

${OBJECTDIR}/SID_layout.o: SID_layout.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_layout.o SID_layout.cpp

${OBJECTDIR}/SID_menu_bar.o: SID_menu_bar.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_menu_bar.o SID_menu_bar.cpp

${OBJECTDIR}/SID_mouse.o: SID_mouse.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_mouse.o SID_mouse.cpp

${OBJECTDIR}/SID_object.o: SID_object.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_object.o SID_object.cpp

${OBJECTDIR}/SID_object_ingame.o: SID_object_ingame.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_object_ingame.o SID_object_ingame.cpp

${OBJECTDIR}/SID_object_template.o: SID_object_template.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_object_template.o SID_object_template.cpp

${OBJECTDIR}/SID_object_templates.o: SID_object_templates.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_object_templates.o SID_object_templates.cpp

${OBJECTDIR}/SID_perlin.o: SID_perlin.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_perlin.o SID_perlin.cpp

${OBJECTDIR}/SID_plot.o: SID_plot.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_plot.o SID_plot.cpp

${OBJECTDIR}/SID_plots.o: SID_plots.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_plots.o SID_plots.cpp

${OBJECTDIR}/SID_radio_button.o: SID_radio_button.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_radio_button.o SID_radio_button.cpp

${OBJECTDIR}/SID_rectangle_2d.o: SID_rectangle_2d.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_rectangle_2d.o SID_rectangle_2d.cpp

${OBJECTDIR}/SID_rectangle_3d.o: SID_rectangle_3d.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_rectangle_3d.o SID_rectangle_3d.cpp

${OBJECTDIR}/SID_regions.o: SID_regions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_regions.o SID_regions.cpp

${OBJECTDIR}/SID_render.o: SID_render.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_render.o SID_render.cpp

${OBJECTDIR}/SID_select_3drect.o: SID_select_3drect.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_select_3drect.o SID_select_3drect.cpp

${OBJECTDIR}/SID_select_delegate.o: SID_select_delegate.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_select_delegate.o SID_select_delegate.cpp

${OBJECTDIR}/SID_select_vbo.o: SID_select_vbo.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_select_vbo.o SID_select_vbo.cpp

${OBJECTDIR}/SID_sidof.o: SID_sidof.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_sidof.o SID_sidof.cpp

${OBJECTDIR}/SID_sidofs.o: SID_sidofs.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_sidofs.o SID_sidofs.cpp

${OBJECTDIR}/SID_structure.o: SID_structure.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_structure.o SID_structure.cpp

${OBJECTDIR}/SID_texture.o: SID_texture.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_texture.o SID_texture.cpp

${OBJECTDIR}/SID_textures.o: SID_textures.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_textures.o SID_textures.cpp

${OBJECTDIR}/SID_timer.o: SID_timer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_timer.o SID_timer.cpp

${OBJECTDIR}/SID_toggle.o: SID_toggle.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_toggle.o SID_toggle.cpp

${OBJECTDIR}/SID_user_events.o: SID_user_events.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_user_events.o SID_user_events.cpp

${OBJECTDIR}/SID_vbo.o: SID_vbo.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_vbo.o SID_vbo.cpp

${OBJECTDIR}/SID_vector3.o: SID_vector3.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_vector3.o SID_vector3.cpp

${OBJECTDIR}/SID_vertex3.o: SID_vertex3.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SID_vertex3.o SID_vertex3.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sid

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
