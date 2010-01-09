#include <monapi.h>
#define MUNIT_GLOBAL_VALUE_DEFINED
#include <monapi/MUnit.h>

using namespace MonAPI;

int main(int argc, char *argv[])
{
    uint64_t msec1 = Date::nowInMsec();
    sleep(100);
    uint64_t msec2 = Date::nowInMsec();
    uint64_t diff = msec2 - msec1;
    EXPECT_TRUE(diff > 70);
    EXPECT_TRUE(diff < 130);

    TEST_RESULTS(monapi_misc);
    return 0;
}
