CFLAGS = -O3 -g3 -Wall -c -fmessage-length=0 -fopenmp -fpermissive -MMD -MP -std=c++11
LIBS = -lopencv_core -lopencv_highgui -lgomp 
IDIR = headers
OPENCVDIR = headers/openCV
IFLAGS = -I$(IDIR) -I$(OPENCVDIR)

all: myFaceDetector

myFaceDetector: myFaceDetector.o
	g++ -o myFaceDetector myFaceDetector.o $(LIBS)
	
myFaceDetector.o: myFaceDetector.cpp
	g++ $(IFLAGS) $(CFLAGS) myFaceDetector.cpp
	
myFaceDetector.cpp: *.h

clean: 
	rm *.o myFaceDetector *.d
