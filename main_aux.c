#include "SPConfig.h"
#include "SPLogger.h"
#include "SPPoint.h"
#include "main_aux.h"
#include <stdlib.h> // malloc, free, NULL
#include <stdio.h> // FILE, stdout, fopen, fclose, sprintf, printf, fflush, stdout
#include <stdbool.h> // bool, true, false
#include <string.h> // strcmp

void getFileName(char* filename, int argc, char** argv){
	if (argc > 3) {
		filename = NULL;
	} else if ((argc == 3) && (argv[1][0] == '-') && (argv[1][1] == 'c')) { // Special configuration file specified by "-c <config_filename>"
		strcpy(filename,argv[2]);
	} else { // Use default configuration file
		strcpy(filename,"spcbir.config");
	}
}
