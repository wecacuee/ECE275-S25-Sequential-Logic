SRC := $(wildcard *.c)

OBJ := ${SRC:.c=.o}#Set flags to compile

CC        :=  gcc
CFLAGS    := -Wall -g 
LDFLAGS	  := # -L. 
LDLIBS    := -lm -lc # Library flags or names
#OPT	  := -O3

all: quine_mc_cluskey

# Suffix for source files
.SUFFIXES : .o .c

#Build binary
quine_mc_cluskey:

#Cleanup objekt files
clean:
	rm -f *.o quine_mc_cluskey  


## Using builtin rules
## 
# %: %.o
# 	$(CC) $(LDFLAGS) n.o $(LDLIBS)
# %.o: %.c
#  	$(CC) $(CPPFLAGS) $(CFLAGS) -c
