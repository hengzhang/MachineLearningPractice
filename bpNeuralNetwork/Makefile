ifdef LONG
INTT = -DLONG
endif

ifdef EDGELONG
INTE = -DEDGELONG
endif

#compilers
PCC = g++
PCFLAGS = -O2 $(INTT) $(INTE)

COMMON= dp_network.h

ALL= demo

all: $(ALL)

% : %.cpp $(COMMON)
	$(PCC) $(PCFLAGS) -o $@ $< 

.PHONY : clean

clean :
	rm -f *.o $(ALL)
