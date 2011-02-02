#include "CuTest.h"
#include "sonLib.h"

CuSuite *matchingAlgorithmsTestSuite(void);

int referenceRunAllTests(void) {
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();
    CuSuiteAddSuite(suite, matchingAlgorithmsTestSuite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
    return suite->failCount > 0;
}

int main(void) {
    int i = referenceRunAllTests();
    //while(1);
    return i;
}