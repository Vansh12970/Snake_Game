CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
SRCDIR = src
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = SnakeGame

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SRCDIR)/*.o $(TARGET)
	
run: $(TARGET)
	./$(TARGET)

install:
	@echo "Installing dependencies..."
	@echo "Make sure you have g++ compiler installed"
	@echo "For Windows: Install MinGW or use Visual Studio"
	@echo "For Linux: sudo apt-get install g++"
	@echo "For macOS: Install Xcode command line tools"