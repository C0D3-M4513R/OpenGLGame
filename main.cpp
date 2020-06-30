#include "Renderer.h"

/*
uint8_t verbose = 0; //verbosity level

  void arginit(int argc, char *argv[]) {
   while (true) {
       static const struct option lopts[] = {
               {"verbose", optional_argument, NULL, 'v'},
               {"help",    no_argument,       NULL, 'h'},
               {NULL,      0,                 NULL, 0},
       };
       int c = getopt_long(argc, argv, "v::h", lopts, NULL);
       if (c == -1)
           break;
       switch (c) {
           case 'v':
               if (optarg)
                   verbose = strtol(optarg, nullptr, 10);
               else
                   verbose = 1;
               break;
           default:
               printf("Wrong Usage! Please follow the help below!\n");
           case 'h':
               printf("Usage: \n");
               for (const option value : lopts) {
                   printf("-%c --%s\n", value.val, value.name);
               }
               exit(0);
       }
   }
}
*/

 [[maybe_unused]]
 int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
//    arginit(argc, argv);
    Renderer::run();
    return 0;
};
