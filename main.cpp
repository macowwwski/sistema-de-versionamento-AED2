#include "CLI.h"

int main(int argc, char* argv[]) {
    CommandLineInterface cli;
    cli.processarArgumentos(argc, argv);
    return 0;
}