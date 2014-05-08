#include <stdio.h>
#include <dlfcn.h>

#define SETUP_STACK                              \
        i = 2;                                       \
        while (++i < argc - 1) {                     \
                printf("i: %d\n", i);\
                switch (argv[i][0]) {                    \
                        case 'i':                            \
                                printf("%d\n", i);\
                                asm volatile("push %0" ::        \
                                "r"(atoi(&argv[i][1])) );             \
                            esp += 4;                        \
                                printf("%d\n", i);\
                                break;                           \
                        case 'd':                            \
                                atof(&argv[i][1]);               \
                                asm volatile("subl $8, %esp\n"   \
                                "fstpl (%esp)" );                \
                                esp += 8;                        \
                            break;                           \
                        case 's':                            \
                                asm volatile("push %0" ::        \
                                "r"(&argv[i][1]) );               \
                            esp += 4;                        \
                            break;                           \
                        default:                             \
                            printf("error argument type");   \
                            goto exit_runso;                 \
                }                                        \
                printf("%d\n", i);\
        }

#define RESOTRE_STACK                      \
        asm volatile("add %0, %%esp"::"r"(esp))


int main(int argc, char* argv[])
{
        void *handle;
        char *error;
        int i;
        int esp = 0;
        void *func;

        handle = dlopen(argv[1], RTLD_NOW);
        if(0 == handle)
        {
                printf("Can't find library: %s\n", argv[1]);
                return -1;
        }

        func = dlsym(handle, argv[2]);
        if ((error = dlerror()) != NULL)
        {
                printf("Find symbol %s error: %s\n", argv[2], error);
                goto exit_runso;
        }

        switch (argv[argc - 1][0])
        {
                case 'i':
                {
                        int (*func_int)() = func;
                        SETUP_STACK;
                        printf("%p\n", func_int);
                        int ret = func_int();
                        RESOTRE_STACK;
                        printf("ret = %d\n", ret);
                        break;
                }
                case 'd':
                {
                        double (*func_double)() = func;
                        SETUP_STACK;
                        double ret = func_double();
                        RESOTRE_STACK;
                        printf("ret = %f\n", ret);
                        break;
                }
                case 's':
                {
                        char* (*func_str)() = func;
                        SETUP_STACK;
                        char *ret = func_str();
                        RESOTRE_STACK;
                        printf("ret = %s\n", ret);
                        break;
                }
                case 'v':
                {
                        void (*func_void)() = func;
                        SETUP_STACK;
                        func_void();
                        RESOTRE_STACK;
                        printf("ret = void");
                        break;
                }
        }/* end of switch */

exit_runso:
        dlclose(handle);
} 
