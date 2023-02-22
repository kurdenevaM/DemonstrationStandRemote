#include "architect/architect.h"

int main(int argc, char* argv[])
{
    Architect architect(nullptr, argc, argv);
    return architect.startApplication();
}
