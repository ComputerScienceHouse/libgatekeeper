#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include <nfc/nfc.h>
#include <nfc/nfc-types.h>
#include <freefare.h>

#include <gatekeeper/gatekeeper.h>

static unsigned char GK_DEFAULT_DESFIRE_KEY[8] = { 0x0 };

int main(int argc, const char *argv[]) {
    int retval = EXIT_SUCCESS;

    // Display libnfc version
    const char *acLibnfcVersion = nfc_version();
    printf("%s uses libnfc %s\n", argv[0], acLibnfcVersion);

    nfc_device *device = NULL;
    MifareTag *tags = NULL;
    MifareDESFireKey default_desfire_key = mifare_desfire_des_key_new(GK_DEFAULT_DESFIRE_KEY);

    nfc_connstring devices[8];
    size_t device_count;

    nfc_context *context;
    nfc_init(&context);
    char *tag_uid = 0;

    if (context == NULL) {
        errx(EXIT_FAILURE, "Unable to init libnfc (malloc)");
    }

    device_count = nfc_list_devices(context, devices, sizeof(devices) / sizeof(*devices));
    if (device_count <= 0) {
        warnx("No NFC device found");
        retval = EXIT_FAILURE;
        goto abort;
    }

    if (!(device = nfc_open(context, devices[0]))) {
        warnx("nfc_open() failed.");
        retval = EXIT_FAILURE;
        goto abort;
    }

    if (!(tags = freefare_get_tags(device))) {
        warnx("Error listing tags.");
        retval = EXIT_FAILURE;
        goto abort_nfc;
    }

    for (size_t i = 0; tags[i]; i++) {
        MifareTag tag = tags[i];

        switch (freefare_get_tag_type(tag)) {
            case DESFIRE:
                break;
            default:
                continue;
        }

        tag_uid = freefare_get_tag_uid(tag);
        printf("Tag with UID %s is a %s\n", tag_uid, freefare_get_tag_friendly_name(tag));

        if(format_tag(tag, /*"047b4e92d65c80", "64d4b8c1960a49986f486c5ea520c975"*/ NULL, NULL) != EXIT_SUCCESS) {
            fprintf(stderr, "Dude what the fuck why my format not working :(\n");
        }
        printf("In theory we're ok?");
        free(tag_uid);
        tag_uid = 0;
    }

    abort_tags:
    freefare_free_tags(tags);

    abort_nfc:
    nfc_close(device);

    abort:
    mifare_desfire_key_free(default_desfire_key);
    nfc_exit(context);
    exit(retval);
}
