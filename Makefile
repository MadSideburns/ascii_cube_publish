all: main


main: ascii_cube.cpp Vector3D.o RotationMatrix.o Face.o Cube.o Ray.o Camera.o Sky.o
	@echo "Linking and producing executable"
	g++ ascii_cube.cpp Vector3D.o RotationMatrix.o Face.o Cube.o Ray.o Camera.o Sky.o -o main


Vector3D.o: Vector3D.cpp
	@echo "Compiling Vector3D file"
	g++ -fpic -c Vector3D.cpp

RotationMatrix.o: RotationMatrix.cpp
	@echo "Compiling RotationMatrix file"
	g++ -fpic -c RotationMatrix.cpp

Face.o: Face.cpp
	@echo "Compiling Face file"
	g++ -fpic -c Face.cpp

Cube.o: Cube.cpp
	@echo "Compiling Cube file"
	g++ -fpic -c Cube.cpp

Ray.o: Ray.cpp
	@echo "Compiling Ray file"
	g++ -fpic -c Ray.cpp

Camera.o: Camera.cpp
	@echo "Compiling Camera file"
	g++ -fpic -c Camera.cpp

Sky.o: Sky.cpp
	@echo "Compiling Sky file"
	g++ -fpic -c Sky.cpp

clean:
	@echo "Removing all compiled files"
	rm *.o main