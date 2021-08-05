INSTALL_BIN = /usr/local/bin/
CXXFLAGS += -std=c++11 -pedantic -Wall -Wextra -Werror -Wunused -Wconversion

ifdef DEBUG
	override DEBUG = -g
endif

mmixal-fmt: mmixal-fmt.cc
	$(CXX) $(CXXFLAGS) $(DEBUG) -o $@ $^

.PHONY: install
install:
	mv mmixal-fmt $(INSTALL_BIN)

.PHONY: clean
clean:
	rm mmixal-fmt

