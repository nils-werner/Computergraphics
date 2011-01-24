CC = g++
LFLAGS = -Wall
SOURCE = A10.3
TARGET = runme

$(TARGET): $(SOURCE).c sunShader.glsl offLoader.h helper.h
	$(CC)  $(LFLAGS) $(SOURCE).c -o $(TARGET) -lglut -lGLU -lGLEW

clean: 
	rm ./$(TARGET)
