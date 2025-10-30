#include <assert.h>
#include "../src/utils.h"

void test_concatenate() {
    // Test cases for concatenate function
    assert(concatenateDigits(1, 2) == 12);
    assert(concatenateDigits(0, 5) == 5);
    assert(concatenateDigits(9, 9) == 99);
    assert(concatenateDigits(2, 0) == 20);
}

void test_convertSecondsToDigits() {
    int d1, d2, d3, d4;

    convertSecondsToDigits(0, &d1, &d2, &d3, &d4);
    assert(d1 == 0 && d2 == 0 && d3 == 0 && d4 == 0);

    convertSecondsToDigits(3661, &d1, &d2, &d3, &d4);
    assert(d1 == 0 && d2 == 1 && d3 == 0 && d4 == 1);

    convertSecondsToDigits(86399, &d1, &d2, &d3, &d4);
    assert(d1 == 2 && d2 == 3 && d3 == 5 && d4 == 9);
}

int main(){
    test_concatenate();
    test_convertSecondsToDigits();

    return 0;
}