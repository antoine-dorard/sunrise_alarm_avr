#include <assert.h>
#include "../src/utils.h"

void test_concatenate() {
    // Test cases for concatenate function
    assert(concatenate(1, 2) == 12);
    assert(concatenate(0, 5) == 5);
    assert(concatenate(9, 9) == 99);
    assert(concatenate(2, 0) == 20);
}

int main(){
    test_concatenate();

    return 0;
}