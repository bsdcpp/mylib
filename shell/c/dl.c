#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

void __attribute__ ((constructor)) x_init(void)
{
	printf("now loaded\n");
}

void __attribute__ ((destructor)) x_fini(void)
{
	printf("over\n");
}

int main(int argc, char **argv) 
{ 
	void *handle; 
	double (*cosine)(double); 
	char *error; 
	printf("================\n");
	handle = dlopen ("/lib/libm.so.6", RTLD_LAZY); 
	if (!handle) { 
		fputs (dlerror(), stderr); 
		exit(1); 
	} 
	printf("-------------------\n");
	cosine = dlsym(handle, "cos"); 
	if ((error = dlerror()) != NULL) { 
		fputs(error, stderr); 
		exit(1); 
	} 
	printf ("%f\n", (*cosine)(2.0)); 
	printf("+++++++++++\n");
	dlclose(handle); 
	printf("end+++++++++++\n");
	return 0; 
} 
