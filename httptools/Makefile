LIB =	httptools

SRCS =				  	\
	http_parser			\
	HttpParser			\
	HttpRequestParser	\
	HttpResponseParser	\
	HttpResponseParser	\
	PyHttpFunc			\
	HttpTools

ifndef KBE_ROOT
export KBE_ROOT := $(subst /kbe/src/lib/$(LIB),,$(CURDIR))
endif

USE_PYTHON=1
include $(KBE_ROOT)/kbe/src/build/common.mak