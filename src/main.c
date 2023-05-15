#include "ubus_utils.h"

int main()
{
    struct ubus_context *ctx;
    if (start_ubus(&ctx) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    ubus_free(ctx);

    return EXIT_SUCCESS;
}
