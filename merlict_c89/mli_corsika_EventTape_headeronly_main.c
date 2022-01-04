#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include "mli_corsika_EventTape_headeronly.h"

int main(int argc, char *argv[])
{
        printf("MLI %d.%d.%d\n",
               MLI_VERSION_MAYOR,
               MLI_VERSION_MINOR,
               MLI_VERSION_PATCH);
        printf("MLI_CORSIKA %d.%d.%d\n",
               MLI_CORSIKA_VERSION_MAYOR,
               MLI_CORSIKA_VERSION_MINOR,
               MLI_CORSIKA_VERSION_PATCH);
        printf("MLI_CORSIKA_EVENTTAPE %d.%d.%d\n",
               MLI_CORSIKA_EVENTTAPE_VERSION_MAYOR,
               MLI_CORSIKA_EVENTTAPE_VERSION_MINOR,
               MLI_CORSIKA_EVENTTAPE_VERSION_PATCH);

        struct mliEventTapeReader tari = mliEventTapeReader_init();
        float bunch[8];
        float corh[273];

        chk_msg(argc == 2, "Expected path to evt.tar.");
        chk(mliEventTapeReader_open(&tari, argv[1]));
        chk(mliEventTapeReader_read_runh(&tari, corh));

        printf("run-number %f\n", corh[MLI_CORSIKA_RUNH_RUN_NUMBER]);

        while (mliEventTapeReader_read_evth(&tari, corh)) {
                /* work on EVTH */
                int b = 0;
                printf("event-number %f\n",
                       corh[MLI_CORSIKA_EVTH_EVENT_NUMBER]);
                while (mliEventTapeReader_read_cherenkov_bunch(&tari, bunch)) {
                        printf("bunch %d: x %fcm, y %fcm\n",
                               b,
                               bunch[0],
                               bunch[1]);
                        b += 1;
                }
                printf("total bunches %d\n", b);
        }
        chk(mliEventTapeReader_close(&tari));
        return EXIT_SUCCESS;
error:
        return EXIT_FAILURE;
}