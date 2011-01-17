CC = g++
LFLAGS = -Wall
SOURCE = A9.3
TARGET = runme


$(TARGET): $(SOURCE).C offLoader.h helper.h
	$(CC)  $(LFLAGS) $(SOURCE).C -o $(TARGET) -lglut -lGLU

clean: 
	rm ./$(TARGET)
