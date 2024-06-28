# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++2a -Wall -Wextra

# Source files
SRCS = VMTranslator.cpp \
       CodeWriter.cpp \
       HelpersVM.cpp \
       Parser.cpp

# Header files
HDRS = CodeWriter.h \
       HelpersVM.h \
       Parser.h \
       ProgramControlCommands.h \
       ArithmeticCommands.h \
       ArithmeticLogicalCommands.h \
       Booting.h \
       BranchingCommands.h \
       LogicalAndOrNot.h \
       LogicalCommandsEQ.h \
       LogicalCommandsGT.h \
       LogicalCommandsLT.h \
       PushConstant.h \
       PushPopCommands.h \
       PushPointer.h \
       PushSegment.h \
       PushStatic.h \
       PushTemp.h \
       WriteCallCommand.h \
       WriteFunctionCommand.h \
       WriteReturnCommand.h

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable file
EXEC = VMTranslator

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
