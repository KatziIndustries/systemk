#define _POSIX_C_SOURCE 200809L

#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
    char **items;
    size_t count;
    size_t capacity;
} StringArray;

static void string_array_free(StringArray *arr)
{
    if (!arr)
        return;

    for (size_t i = 0; i < arr->count; i++)
        free(arr->items[i]);

    free(arr->items);

    arr->items = NULL;
    arr->count = 0;
    arr->capacity = 0;
}

static int string_array_push(StringArray *arr, const char *str)
{
    if (arr->count == arr->capacity) {
        size_t new_capacity = arr->capacity ? arr->capacity * 2 : 16;

        char **tmp = realloc(arr->items,
                             new_capacity * sizeof(*tmp));
        if (!tmp)
            return -1;

        arr->items = tmp;
        arr->capacity = new_capacity;
    }

    arr->items[arr->count] = strdup(str);
    if (!arr->items[arr->count])
        return -1;

    arr->count++;
    return 0;
}

static int has_suffix(const char *str, const char *suffix)
{
    size_t len = strlen(str);
    size_t suffix_len = strlen(suffix);

    return len >= suffix_len &&
           strcmp(str + len - suffix_len, suffix) == 0;
}

static char *trim(char *s)
{
    while (*s == ' ' || *s == '\t' ||
           *s == '\r' || *s == '\n')
        s++;

    char *end = s + strlen(s);

    while (end > s &&
           (end[-1] == ' ' || end[-1] == '\t' ||
            end[-1] == '\r' || end[-1] == '\n'))
        end--;

    *end = '\0';
    return s;
}

int scan_service_wants(StringArray *result)
{
    DIR *dir = opendir("/etc/systemd/system");
    if (!dir) {
        perror("opendir");
        return -1;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;

        if (!has_suffix(entry->d_name, ".service.wants"))
            continue;

        char wants_path[PATH_MAX];

        if (snprintf(wants_path,
                     sizeof(wants_path),
                     "%s/%s",
                     "/etc/systemd/system",
                     entry->d_name) >= (int)sizeof(wants_path)) {
            fprintf(stderr, "Path too long: %s\n", entry->d_name);
            continue;
        }

        struct stat st;

        if (stat(wants_path, &st) != 0)
            continue;

        if (!S_ISDIR(st.st_mode))
            continue;

        DIR *wants_dir = opendir(wants_path);
        if (!wants_dir)
            continue;

        struct dirent *wanted;

        while ((wanted = readdir(wants_dir)) != NULL) {
            if (strcmp(wanted->d_name, ".") == 0 ||
                strcmp(wanted->d_name, "..") == 0)
                continue;

            if (string_array_push(result, wanted->d_name) != 0) {
                closedir(wants_dir);
                closedir(dir);
                return -1;
            }
        }

        closedir(wants_dir);
    }

    closedir(dir);
    return 0;
}

int read_file(const char *path, char **out, size_t *out_len)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        perror(path);
        return -1;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return -1;
    }

    long size = ftell(fp);
    if (size < 0) {
        fclose(fp);
        return -1;
    }

    rewind(fp);

    char *buffer = malloc((size_t)size + 1);
    if (!buffer) {
        fclose(fp);
        return -1;
    }

    size_t n = fread(buffer, 1, (size_t)size, fp);

    if (ferror(fp)) {
        free(buffer);
        fclose(fp);
        return -1;
    }

    buffer[n] = '\0';

    fclose(fp);

    *out = buffer;

    if (out_len)
        *out_len = n;

    return 0;
}

int service()
{
    StringArray services = {0};

    if (scan_service_wants(&services) != 0) {
        fprintf(stderr, "Failed to scan wants directories\n");
        return 1;
    }

    printf("Found %zu wanted units:\n\n", services.count);

    for (size_t i = 0; i < services.count; i++)
        printf("[%zu] %s\n", i, services.items[i]);

    return 0;
}


void start_service(char *name)
{

}