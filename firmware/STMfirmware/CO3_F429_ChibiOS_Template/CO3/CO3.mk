#  This file is part of the CO3 runtime.
# 
#  The CO3 runtime is free software: you can redistribute it and/or modify it
#  under the terms of the GNU Lesser General Public License as published by the
#  Free Software Foundation, either version 3 of the License, or (at your
#  option) any later version.
# 
#  The CO3 runtime is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
#  for more details.
# 
#  You should have received a copy of the GNU Lesser General Public License
#  along with CO3. If not, see <https://www.gnu.org/licenses/>.

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
