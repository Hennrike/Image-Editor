#include <stdio.h>
#include <stdint.h>
#include "image.h"

int main(int argc, char const *argv[]) {
    read("../images/in/TestImage4.bmp");
    convertBW();
    write("../images/out/TestImage.bmp");
    clear();
    
    return 0;
}
