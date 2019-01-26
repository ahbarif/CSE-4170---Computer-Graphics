all:
	g++ main.cpp -o main
	./main
cpp14:
	g++ -std=c++1y main.cpp -o main
	./main
opengl: 
	g++ main.cpp -o firstOpenGlApp -lglut -lGLU -lGL
	./firstOpenGlApp
