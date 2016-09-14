CC = gcc
CPP = g++
# put your object files here
OBJS = main.o SPImageProc.o main_aux.o SPPoint.o SPLogger.o SPListElement.o SPList.o SPBPriorityQueue.o \
sort_r.o SPConfig.o Extraction.o KDTreeNode.o SPKDArray.o
# The executable filename
EXEC = SPCBIR
INCLUDEPATH=/usr/local/lib/opencv-3.1.0/include/
LIBPATH=/usr/local/lib/opencv-3.1.0/lib/
LIBS=-lopencv_xfeatures2d -lopencv_features2d \
-lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core

CPP_COMP_FLAG = -std=c++11 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

C_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(CPP) $(OBJS) -L$(LIBPATH) $(LIBS) -o $@
# C++ Files
main.o: main.cpp SPImageProc.h Extraction.h main_aux.h SPConfig.h SPLogger.h SPPoint.h defines.h KDTreeNode.h
	$(CPP) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
SPImageProc.o: SPImageProc.cpp SPImageProc.h SPConfig.h SPPoint.h SPLogger.h
	$(CPP) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
# C Files
main_aux.o: main_aux.c main_aux.h SPConfig.h SPLogger.h SPPoint.h defines.h KDTreeNode.h Extraction.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPPoint.o: SPPoint.c SPPoint.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPLogger.o: SPLogger.c SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPListElement.o: SPListElement.c SPListElement.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPList.o: SPList.c SPList.h SPListElement.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPBPriorityQueue.o: SPBPriorityQueue.c SPBPriorityQueue.h SPList.h SPListElement.h
	$(CC) $(C_COMP_FLAG) -c $*.c
sort_r.o: sort_r.c sort_r.h 
	$(CC) $(C_COMP_FLAG) -c $*.c
SPConfig.o: SPConfig.c SPConfig.h SPLogger.h defines.h
	$(CC) $(C_COMP_FLAG) -c $*.c
Extraction.o: Extraction.c Extraction.h SPLogger.h SPPoint.h main_aux.h defines.h
	$(CC) $(C_COMP_FLAG) -c $*.c
KDTreeNode.o: KDTreeNode.c KDTreeNode.h SPConfig.h SPKDArray.h SPPoint.h defines.h SPBPriorityQueue.h \
SPListElement.h SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPKDArray.o: SPKDArray.c SPKDArray.h SPPoint.h sort_r.h defines.h SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
