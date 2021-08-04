INSTALL_BIN = /usr/local/bin/
CXXFLAGS += -std=c++11 -Wall -Wextra -Werror -Wstrict-prototypes

ifdef DEBUG
	override DEBUG=-g
endif

mmixal-fmt: mmixal-fmt.cc
	$(CXX) $(CXXFLAGS) $(DEBUG) -o $@ $^

.PHONY: install
install:
	mv mmixal-fmt $(INSTALL_BIN)

.PHONY: clean
clean:
	rm mmixal-fmt

