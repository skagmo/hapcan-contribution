#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=main.c platform.c uart.c helpers.c scheduler.c _string.c _delay.c config.c can.c dali.c logic.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.o ${OBJECTDIR}/platform.o ${OBJECTDIR}/uart.o ${OBJECTDIR}/helpers.o ${OBJECTDIR}/scheduler.o ${OBJECTDIR}/_string.o ${OBJECTDIR}/_delay.o ${OBJECTDIR}/config.o ${OBJECTDIR}/can.o ${OBJECTDIR}/dali.o ${OBJECTDIR}/logic.o
POSSIBLE_DEPFILES=${OBJECTDIR}/main.o.d ${OBJECTDIR}/platform.o.d ${OBJECTDIR}/uart.o.d ${OBJECTDIR}/helpers.o.d ${OBJECTDIR}/scheduler.o.d ${OBJECTDIR}/_string.o.d ${OBJECTDIR}/_delay.o.d ${OBJECTDIR}/config.o.d ${OBJECTDIR}/can.o.d ${OBJECTDIR}/dali.o.d ${OBJECTDIR}/logic.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/main.o ${OBJECTDIR}/platform.o ${OBJECTDIR}/uart.o ${OBJECTDIR}/helpers.o ${OBJECTDIR}/scheduler.o ${OBJECTDIR}/_string.o ${OBJECTDIR}/_delay.o ${OBJECTDIR}/config.o ${OBJECTDIR}/can.o ${OBJECTDIR}/dali.o ${OBJECTDIR}/logic.o

# Source Files
SOURCEFILES=main.c platform.c uart.c helpers.c scheduler.c _string.c _delay.c config.c can.c dali.c logic.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX564F128H
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
${OBJECTDIR}/platform.o: platform.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/platform.o.d 
	@${RM} ${OBJECTDIR}/platform.o 
	@${FIXDEPS} "${OBJECTDIR}/platform.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/platform.o.d" -o ${OBJECTDIR}/platform.o platform.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
${OBJECTDIR}/uart.o: uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/uart.o.d 
	@${RM} ${OBJECTDIR}/uart.o 
	@${FIXDEPS} "${OBJECTDIR}/uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/uart.o.d" -o ${OBJECTDIR}/uart.o uart.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
${OBJECTDIR}/helpers.o: helpers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/helpers.o.d 
	@${RM} ${OBJECTDIR}/helpers.o 
	@${FIXDEPS} "${OBJECTDIR}/helpers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/helpers.o.d" -o ${OBJECTDIR}/helpers.o helpers.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
${OBJECTDIR}/scheduler.o: scheduler.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/scheduler.o.d 
	@${RM} ${OBJECTDIR}/scheduler.o 
	@${FIXDEPS} "${OBJECTDIR}/scheduler.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/scheduler.o.d" -o ${OBJECTDIR}/scheduler.o scheduler.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
${OBJECTDIR}/_string.o: _string.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/_string.o.d 
	@${RM} ${OBJECTDIR}/_string.o 
	@${FIXDEPS} "${OBJECTDIR}/_string.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/_string.o.d" -o ${OBJECTDIR}/_string.o _string.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
${OBJECTDIR}/_delay.o: _delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/_delay.o.d 
	@${RM} ${OBJECTDIR}/_delay.o 
	@${FIXDEPS} "${OBJECTDIR}/_delay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/_delay.o.d" -o ${OBJECTDIR}/_delay.o _delay.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
${OBJECTDIR}/config.o: config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/config.o.d 
	@${RM} ${OBJECTDIR}/config.o 
	@${FIXDEPS} "${OBJECTDIR}/config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/config.o.d" -o ${OBJECTDIR}/config.o config.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
${OBJECTDIR}/can.o: can.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/can.o.d 
	@${RM} ${OBJECTDIR}/can.o 
	@${FIXDEPS} "${OBJECTDIR}/can.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/can.o.d" -o ${OBJECTDIR}/can.o can.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
${OBJECTDIR}/dali.o: dali.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/dali.o.d 
	@${RM} ${OBJECTDIR}/dali.o 
	@${FIXDEPS} "${OBJECTDIR}/dali.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/dali.o.d" -o ${OBJECTDIR}/dali.o dali.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
${OBJECTDIR}/logic.o: logic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/logic.o.d 
	@${RM} ${OBJECTDIR}/logic.o 
	@${FIXDEPS} "${OBJECTDIR}/logic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/logic.o.d" -o ${OBJECTDIR}/logic.o logic.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
else
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
${OBJECTDIR}/platform.o: platform.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/platform.o.d 
	@${RM} ${OBJECTDIR}/platform.o 
	@${FIXDEPS} "${OBJECTDIR}/platform.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/platform.o.d" -o ${OBJECTDIR}/platform.o platform.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
${OBJECTDIR}/uart.o: uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/uart.o.d 
	@${RM} ${OBJECTDIR}/uart.o 
	@${FIXDEPS} "${OBJECTDIR}/uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/uart.o.d" -o ${OBJECTDIR}/uart.o uart.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
${OBJECTDIR}/helpers.o: helpers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/helpers.o.d 
	@${RM} ${OBJECTDIR}/helpers.o 
	@${FIXDEPS} "${OBJECTDIR}/helpers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/helpers.o.d" -o ${OBJECTDIR}/helpers.o helpers.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
${OBJECTDIR}/scheduler.o: scheduler.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/scheduler.o.d 
	@${RM} ${OBJECTDIR}/scheduler.o 
	@${FIXDEPS} "${OBJECTDIR}/scheduler.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/scheduler.o.d" -o ${OBJECTDIR}/scheduler.o scheduler.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
${OBJECTDIR}/_string.o: _string.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/_string.o.d 
	@${RM} ${OBJECTDIR}/_string.o 
	@${FIXDEPS} "${OBJECTDIR}/_string.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/_string.o.d" -o ${OBJECTDIR}/_string.o _string.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
${OBJECTDIR}/_delay.o: _delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/_delay.o.d 
	@${RM} ${OBJECTDIR}/_delay.o 
	@${FIXDEPS} "${OBJECTDIR}/_delay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/_delay.o.d" -o ${OBJECTDIR}/_delay.o _delay.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
${OBJECTDIR}/config.o: config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/config.o.d 
	@${RM} ${OBJECTDIR}/config.o 
	@${FIXDEPS} "${OBJECTDIR}/config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/config.o.d" -o ${OBJECTDIR}/config.o config.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
${OBJECTDIR}/can.o: can.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/can.o.d 
	@${RM} ${OBJECTDIR}/can.o 
	@${FIXDEPS} "${OBJECTDIR}/can.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/can.o.d" -o ${OBJECTDIR}/can.o can.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
${OBJECTDIR}/dali.o: dali.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/dali.o.d 
	@${RM} ${OBJECTDIR}/dali.o 
	@${FIXDEPS} "${OBJECTDIR}/dali.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/dali.o.d" -o ${OBJECTDIR}/dali.o dali.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
${OBJECTDIR}/logic.o: logic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/logic.o.d 
	@${RM} ${OBJECTDIR}/logic.o 
	@${FIXDEPS} "${OBJECTDIR}/logic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mcci -MMD -MF "${OBJECTDIR}/logic.o.d" -o ${OBJECTDIR}/logic.o logic.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wshadow -Wundef -Wcast-align -Wextra -Wno-unused-variable -Wall -Wno-pointer-sign
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC024FF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=_min_heap_size=2048,--no-code-in-dinit,--no-dinit-in-serial-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/firmware.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=2048,--no-code-in-dinit,--no-dinit-in-serial-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/firmware.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
