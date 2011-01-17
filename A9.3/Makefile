CC = g++
LFLAGS = -Wall
SOURCE = A9.3
TARGET = runme


$(TARGET): $(SOURCE).c offLoader.h helper.h
	$(CC)  $(LFLAGS) $(SOURCE).c -o $(TARGET) -lglut -lGLU

clean: 
	rm ./$(TARGET)
