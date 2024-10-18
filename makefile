CXX = g++
CXXFLAGS = -O2 -Wall -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough \
            -Werror=format-security -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 \
            -D_GLIBCXX_ASSERTIONS \
            -fstack-clash-protection -fstack-protector-strong \
            -Wl,-z,nodlopen -Wl,-z,noexecstack \
            -Wl,-z,relro -Wl,-z,now \
            -Wl,--as-needed -Wl,--no-copy-dt-needed-entries
 
SOURCES = main.cpp # Add your source files here
OBJECTS = $(SOURCES:.cpp=.o)
 
all: my_program
 
my_program: $(OBJECTS)
    $(CXX) $(CXXFLAGS) -o $@ $^
 
%.o: %.cpp
    $(CXX) $(CXXFLAGS) -c $<
 
clean:
    rm -f $(OBJECTS) my_program
