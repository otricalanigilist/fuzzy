# Имя исполняемого файла
TARGET = server

# Компилятор
CXX = g++

# Флаги компиляции
CXXFLAGS = -std=c++17 -Wall -O2

# Заголовки
INCLUDES = -I.

# Исходные и объектные файлы
SRCS = server.cpp alg.cpp
OBJS = $(SRCS:.cpp=.o)

# Сборка по умолчанию
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

# Правило компиляции
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Очистка
clean:
	rm -f $(TARGET) $(OBJS)
