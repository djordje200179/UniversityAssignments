struct args {
	const char* input_file_path;
	const char* output_file_path;
};

struct args read_args(int argc, char** argv);