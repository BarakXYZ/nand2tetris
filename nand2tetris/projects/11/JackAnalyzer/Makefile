# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++2a -Wall -Wextra

# Source files
SRCS = JackAnalyzer.cpp \
       JackTokenizer.cpp \
       CompilationEngine.cpp \
       AnalyzerUtils.cpp \
       SymbolTable.cpp \
       VMWriter.cpp

# Header files
HDRS = JackAnalyzer.h \
       JackTokenizer.h \
       CompilationEngine.h \
       AnalyzerUtils.h \
       SymbolTable.h \
       VMWriter.h


# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable file
EXEC = JackAnalyzer

# Default rule
all: $(EXEC)

# Rule to link object files to create the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile source files into object files
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove object files and executable
clean:
	rm -f $(OBJS) $(EXEC)

# Phony targets
.PHONY: all clean
