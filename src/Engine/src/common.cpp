#include <common.h>
unsigned char *read_file_and_null_terminate(const char *path) {
	FILE *f = fopen(path, "rb");
	if (!f) return 0;

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);

	unsigned char *result = (unsigned char *)malloc(size + 1);
	if (!result) {
		fclose(f);
		return 0;
	}

	fread(result, size, 1, f);
	result[size] = 0;

	fclose(f);
	return result;
}
