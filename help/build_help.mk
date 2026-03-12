# ============================================================================
#  Name	 : build_help.mk
#  Part of  : Squirrel
# ============================================================================
#  Name	 : build_help.mk
#  Part of  : Squirrel
#
#  Description: This make file will build the application help file (.hlp)
# 
# ============================================================================

do_nothing :
	@rem do_nothing

# build the help from the MAKMAKE step so the header file generated
# will be found by cpp.exe when calculating the dependency information
# in the mmp makefiles.

MAKMAKE : Squirrel_0xebfcc31a.hlp
Squirrel_0xebfcc31a.hlp : Squirrel.xml Squirrel.cshlp Custom.xml
	cshlpcmp.bat Squirrel.cshlp
ifeq (WINSCW,$(findstring WINSCW, $(PLATFORM)))
	md $(EPOCROOT)epoc32\$(PLATFORM)\c\resource\help
	copy Squirrel_0xebfcc31a.hlp $(EPOCROOT)epoc32\$(PLATFORM)\c\resource\help
endif

BLD : do_nothing

CLEAN :
	del Squirrel_0xebfcc31a.hlp
	del Squirrel_0xebfcc31a.hlp.hrh

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing
		
FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo Squirrel_0xebfcc31a.hlp

FINAL : do_nothing
