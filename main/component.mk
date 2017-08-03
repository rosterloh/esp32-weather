THISDIR:=$(dir $(abspath $(lastword $(MAKEFILE_LIST))))
DATETIME := $(shell date "+%Y-%b-%d_%H:%M:%S_%Z")

COMPONENT_ADD_INCLUDEDIRS := include
CFLAGS += -DBUID_TIME=\"$(DATETIME)\" -Wno-error=format=
