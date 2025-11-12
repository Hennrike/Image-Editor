#include <stdio.h>
#include <stdint.h>
#include "image.h"

int main(int argc, char const *argv[])
{
    read("../images/TestImageIN3.bmp");
    write("../images/TestImageOUT.bmp");
    clear();
    
    return 0;
}
