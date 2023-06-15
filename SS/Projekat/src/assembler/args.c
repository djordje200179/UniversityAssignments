#include "assembler/args.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

struct args read_args(int argc, char** argv) {
	struct args args;
	args.output_file_path = "a.out";

	opterr = 0;
	char param;
	while ((param = getopt(argc, argv, "o:")) != -1) {
		switch (param) {
			case 'o':
				args.output_file_path = optarg;
				break;
			case '?':
				if (optopt == 'o')
					fprintf(stderr, "Option -o requires an argument.\n");
				else
					fprintf(stderr, "Unknown option `-%c'.\n", optopt);

				// FIXME: Convert exit to try-catch
				exit(1);
			default:
				abort();
		}
	}

	int leftover_args_count = argc - optind;
	if (leftover_args_count != 1) {
		// FIXME: Convert exit to try-catch
		fprintf(stderr, "Expected 1 argument, got %d\n", leftover_args_count);
		exit(1);
	}
	args.input_file_path = argv[optind];

	return args;
}