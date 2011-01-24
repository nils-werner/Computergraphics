CC = g++
LFLAGS = -Wall
SOURCE = A10.3
TARGET = runme

$(TARGET): $(SOURCE).C sunShader.glsl
	$(CC)  $(LFLAGS) $(SOURCE).C -o $(TARGET) -lglut -lGLU -lGLEW

clean: 
	rm ./$(TARGET)
