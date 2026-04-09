#include <stdio.h>
#include <string.h>

int check_password(char *input) {
    char *secret = "RE2026";
    if (strcmp(input, secret) == 0) {
        return 1;
    }
    return 0;
}

int main() {
    char buffer[64];
    printf("Enter password: ");
    scanf("%63s", buffer);

    if (check_password(buffer)) {
        printf("Access granted!\n");
    } else {
        printf("Access denied!\n");
    }
    return 0;
}
