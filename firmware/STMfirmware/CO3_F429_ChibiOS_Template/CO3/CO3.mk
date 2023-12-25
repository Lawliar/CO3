CO3PROTOCOLDIR := $(COMMONDIR)/Protocol
CO3RUNTIMEDIR  := $(COMMONDIR)/SymRuntime
CO3SRC := $(CO3PROTOCOLDIR)/monitor.c \
          $(CO3PROTOCOLDIR)/protocol.c \
          $(CO3PROTOCOLDIR)/ring.c \
          $(CO3RUNTIMEDIR)/runtime.c \
          $(CO3RUNTIMEDIR)/wrapped_malloc.c 


# Required include directories
CO3INC = $(CO3RUNTIMEDIR) \
         $(CO3PROTOCOLDIR)

# Shared variables
ALLCSRC += $(CO3SRC)
ALLINC  += $(CO3INC)
