#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
//ADD ADDITIONAL DEFINES HERE
#define init_module(module_image, len, param_values) syscall(__NR_init_module, module_image, len, param_values)
#define delete_module(name, flags) syscall(__NR_delete_module, name, flags)

int addition(long* result, int input1, int input2);
int substraction(long* result, int input1, int input2);
int multiplication(long* result, int input1, int input2);
int LoadModule(const char* params);
int UnLoadModule();
void SetParamString(char* parm, int input1, int input2, const char* op);
long GetResult();
// StudentID = 109550073 // replace with your student ID
// ADD ADDITIONAL VARIABLES HERE
// You must load and unload the "calculatorModule.ko" module.
const char *moduleName = "calculatorModule.ko";
const char *moduleNameNoExtension = "calculatorModule";
int fd, use_finit;
size_t image_size;
struct stat st;
void *image;

int main(int argc, char** argv){
    long result = -9999999;
    long resultError = -9999999;
    if (argc != 4) {
        printf("%ld\n", resultError);
        return resultError;
    }
    int param01 = atoi(argv[1]);
    int param02 = atoi(argv[2]);
    int operationError = 1;
    if (strcmp(argv[3], "add") == 0) {
        operationError = addition(&result, param01, param02);
    }
    else if (strcmp(argv[3], "sub") == 0) {
        operationError = substraction(&result, param01, param02);
    }
    else if (strcmp(argv[3], "mul") == 0) {
        operationError = multiplication(&result, param01, param02);
    }
    if (operationError == EXIT_SUCCESS) {
        printf("%ld\n", result);
    }
    else {
        printf("%ld\n", resultError);
    }
    return 0;
}
int addition(long* result, int input1, int input2)
{
    *result = 0;
    int operationError = 0;
    //INSERT YOUR CODE HERE
    char *paramsNew = malloc(100);
    SetParamString(paramsNew, input1, input2, "add");
    LoadModule(paramsNew);
    *result = GetResult();
    UnLoadModule();
    return operationError;
}
int substraction(long* result, int input1, int input2)
{
    *result = 0;
    int operationError = 0;
    //INSERT YOUR CODE HERE
    char *paramsNew = malloc(100);
    SetParamString(paramsNew, input1, input2, "sub");
    LoadModule(paramsNew);
    *result = GetResult();
    UnLoadModule();
    return operationError;
}
int multiplication(long* result, int input1, int input2)
{
    *result = 0;
    int operationError = 0;
    //INSERT YOUR CODE HERE
    char *paramsNew = malloc(100);
    SetParamString(paramsNew, input1, input2, "mul");
    LoadModule(paramsNew);
    *result = GetResult();
    UnLoadModule();
    return operationError;
}
void SetParamString(char* parm, int input1, int input2, const char* op) {
    //INSERT YOUR CODE HERE
    sprintf(parm, "firstParam=%d secondParam=%d operationParam=%s", input1, input2, op);
}
int LoadModule(const char* params) {
    //INSERT YOUR CODE HERE
    fd = open(moduleName, O_RDONLY);
    fstat(fd, &st);
    image_size = st.st_size;
    image = malloc(image_size);
    read(fd, image, image_size);
    if (init_module(image, image_size, params) != 0) {
        perror("init_module");
	return EXIT_FAILURE;
    }
    return 0;
}
long GetResult() {
    long result = 0;
    //INSERT YOUR CODE HERE
    FILE *fptr = fopen("/sys/module/calculatorModule/parameters/resultParam", "r");
    if (fptr == NULL){
       printf("Error! opening file"); // Program exits if the file pointer returns NULL.
       exit(1);
   }
    fscanf(fptr,"%ld", &result);
    fclose(fptr);
    return result;
}
int UnLoadModule() {
    //INSERT YOUR CODE HERE
    if (delete_module(moduleNameNoExtension, O_NONBLOCK) != 0) {
	perror("delete_module");
	return EXIT_FAILURE;
    }
    close(fd);
    free(image);
    return 0;
}
