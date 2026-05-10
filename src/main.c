#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv){
	if (argc > 1) {
		fprintf(stderr, "Error: Program needs more than one argument.\n");
		return 1;
	}
	char* filename = NULL;
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
			return 0;
		}
		else if(strcmp(argv[i], "-f") == 0) {
			if (i + 1 < argc) {
				filename = argv[i + 1];
				i++;
			} else {
				fprintf(stderr, "Error: -f option requires a filename argument.\n");
				return 1;
			}
		}
		else {
			fprintf(stderr, "Error: Unknown argument '%s'.\n", argv[i]);
			return 1;
		}
	}
	
	FILE* file = fopen(filename, "r");
	
	if (file == NULL) {
		fprintf(stderr, "Error: Could not open file '%s'.\n", filename);
		return 1;
	}
	
	return 0;
}
