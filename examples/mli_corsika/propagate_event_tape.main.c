#include <stdio.h>
#include <stdlib.h>
#include "../../src/prng/prng.h"
#include "../../src/scenery/scenery.h"
#include "../../src/scenery/scenery_tar.h"
#include "../../src/photon/photon.h"
#include "../../src/photon/photon_interaction.h"
#include "../../src/photon/photon_interaction_vector.h"
#include "../../src/photon/photon_propagation.h"
#include "../../src/corsika/CorsikaPhotonBunch.h"
#include "../../src/corsika/EventTape.h"
#include "../../src/chk/chk.h"
#include "../../src/bool/bool.h"
#include "../../src/string/string_numbers.h"
#include "../../src/math/math.h"
#include "../../src/io/io.h"
#include "../../src/io/io_text.h"
#include "../../src/tar/tar.h"
#include "../../src/tar/tar_io.h"
#include "../../src/cstr/cstr.h"
#include "../../src/cstr/cstr_numbers.h"
#include "../../src/vector/vector.h"


int main(int argc, char *argv[])
{
        uint64_t seed = 0;
        struct mli_IO istream = mli_IO_init();
        struct mli_Prng prng = mli_Prng_init_MT19937(seed);
        struct mli_Scenery scenery = mli_Scenery_init();
        struct mliEventTapeReader run = mliEventTapeReader_init();
        struct mli_corsika_PhotonBunch bunch;
        const double production_distance_offset = 1e2;
        double shower_core_x_m = 0.0;
        double shower_core_y_m = 0.0;
        const uint64_t MAX_ITERATIONS = 128;
        uint64_t sensor_id = 1;
        float runh[273] = {0.0};
        float evth[273] = {0.0};
        float rawbunch[8] = {0.0};

        chk_msg(
                argc == 7,
                "Expected 6 arguments:\n"
                "1) scenery-path\n"
                "2) corsika-run-path\n"
                "3) shower-core-x/m\n"
                "4) shower-core-y/m\n"
                "5) random-seed\n"
                "6) sensor-id");

        chk_msg(
                mli_cstr_ends_with(argv[1], ".tar"),
                "Expected scenery-path to end with '.tar'");
        chk_msg(
                mli_Scenery__from_path_cstr(&scenery, argv[1]),
                "Can not read scenery from '.tar'.");
        chk_msg(
                mli_cstr_ends_with(argv[2], ".tar"),
                "Expected corsika-run-path (TarIo) to end with '.tar'");

        chk_msg(mli_IO_open_file_cstr(&istream, argv[2], "rb"), "Can not open path.");
        chk_msg(mliEventTapeReader_begin(&run, &istream), "Can not open run.");

        chk_msg(
                mli_cstr_to_double(&shower_core_x_m, argv[3]),
                "Can not parse shower-core-x/m");
        chk_msg(
                mli_cstr_to_double(&shower_core_y_m, argv[4]),
                "Can not parse shower-core-y/m");

        chk_msg(
                mli_cstr_to_uint64(&seed, argv[5], 10),
                "Can not parse seed from argv[5].");

        chk_msg(
                mli_cstr_to_uint64(&sensor_id, argv[6], 10),
                "Can not parse sensor id from argv[6].");

        prng = mli_Prng_init_MT19937(seed);

        chk_msg(mliEventTapeReader_read_runh(&run, runh), "Can not read RUNH");

        while (mliEventTapeReader_read_evth(&run, evth)) {
                uint64_t i = 0;

                while (mliEventTapeReader_read_cherenkov_bunch(&run, rawbunch)) {
                        mli_corsika_PhotonBunch_set_from_raw(&bunch, rawbunch);
                        uint64_t idx_final_interaction = 0;
                        uint64_t jj = 0;
                        struct mli_PhotonInteraction final_interaction;
                        struct mli_Photon photon = mli_corsika_PhotonBunch_to_merlict_photon(
                                bunch,
                                production_distance_offset,
                                i
                        );
                        i ++;
                        photon.ray.support.x -= shower_core_x_m;
                        photon.ray.support.y -= shower_core_y_m;

                        struct mli_PhotonInteractionVector photon_history = mli_PhotonInteractionVector_init();
                        chk(mli_PhotonInteractionVector_malloc(&photon_history, MAX_ITERATIONS));

                        chk_msg(
                                mli_propagate_photon(
                                        &scenery,
                                        &photon_history,
                                        &photon,
                                        &prng,
                                        MAX_ITERATIONS),
                                "Failed to propagate."
                        );

                        idx_final_interaction = photon_history.size - 1;
                        final_interaction = photon_history.array[idx_final_interaction];

                        if (final_interaction.on_geometry_surface) {
                                uint64_t robj = final_interaction.geometry_id.robj;
                                uint64_t id = scenery.geometry.robject_ids[robj];
                                /*fprintf(stdout, "id %d\n", id);*/
                                if (id == sensor_id) {

                                        fprintf(
                                                stdout,
                                                "%d,%u,%e,%e\n",
                                                (int)(evth[2-1]),
                                                i,
                                                final_interaction.position_local.x,
                                                final_interaction.position_local.y);
                                }
                        }

                        mli_PhotonInteractionVector_free(&photon_history);
                }
        }

        chk_msg(
                mliEventTapeReader_finalize(&run),
                "Can't finalize EventTape"
        );
        mli_IO_close(&istream);
        mli_Scenery_free(&scenery);
        return EXIT_SUCCESS;
chk_error:
        return EXIT_FAILURE;
}
