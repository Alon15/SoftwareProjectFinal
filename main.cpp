#include "main_aux.h"
#include "SPImageProc.h"
#include "SPConfig.h"
#include <stdio.h> // printf, stdout, scanf, snprintf
#include <stdlib.h> // malloc, EXIT_FAILURE, EXIT_SUCCESS
#include <string.h> // strcmp

int main ( int argc, char *argv[] ){
	char* filename;
	SP_CONFIG_MSG msg = SP_CONFIG_SUCCESS;
	SPConfig config;
	sp::ImageProc *imageProc = NULL;
	if (argc > 2)
		filename = NULL;
	else if (argc == 2)
		filename = argv[1];
	else
		filename = (char*)"spcbir.config";
	config = spConfigCreate(filename, &msg);

	// TODO activate the logger
	try{
	imageProc = new sp::ImageProc(config);
	}
	catch(...){
		//TODO ImageProc error, terminate
	}


	// TODO Main code here


	delete imageProc;
	spConfigDestroy(config);

	return (EXIT_SUCCESS);
}
