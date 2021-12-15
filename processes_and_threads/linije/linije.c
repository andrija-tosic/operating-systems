#include <string.h>
#include <pthread.h>
#include <stdio.h>

struct arg {
	char* datoteka;
	int linije;
};

void* thr(void* arg) {
	struct arg* args = (struct arg*)arg;

	FILE *f = fopen(args->datoteka, "r");

	char red[80];
	int l = args->linije;
	for (int i=0; i<l; i++) {
		fgets(red, 80, f);

		red[0] = tolower(red[0]);
		for (int j=1; red[j] != '\0'; j++) {
			red[j] = toupper(red[j]);
		}

		printf("%s\n", red);
	}
	fclose(f);
}

int main(int argc, char* argv[]) {
	struct arg args;
	args.datoteka = argv[1];
	args.linije = atoi(argv[2]);

	printf("%d\n", args.linije);

	pthread_t thread;
	pthread_create(&thread, NULL, thr, &args);
	pthread_join(thread, NULL);
}