#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "toml_format.h"
#include "run_bin.h"

void run_bin(package_t *package, bin_t *bin) {
    // Make the required target dirs
    char *target_dir_path = malloc(sizeof(char) * (strlen(bin->targetdir) + strlen(package->name) + 2));
    assert (target_dir_path != NULL);
    sprintf(target_dir_path, "%s/%s", bin->targetdir, package->name);
    char *mkdir_f = "mkdir -p %s/obj %s/bin";
    char *make_dirs_command = malloc(sizeof(char) * (strlen(mkdir_f) + strlen(target_dir_path) * 2 + 2));
    sprintf(make_dirs_command, mkdir_f, target_dir_path, target_dir_path);
    system(make_dirs_command);
    free(make_dirs_command);

    // Make the objs
    char *obj_f = "gcc -O2 -c $(find %s -name \"*.c\") -I $(find %s -type d | grep \"*.h\")";
    char *compile_objs_command = malloc(sizeof(char) * (strlen(obj_f) + strlen(bin->includedir) + strlen(bin->srcdir) + 1));
    assert(compile_objs_command != NULL);
    sprintf(compile_objs_command, obj_f, bin->srcdir, bin->includedir);
    system(compile_objs_command);
    free(compile_objs_command);

    // Move all the objs
    char *move_f = "mv *.o %s/obj";
    char *move_objs_command = malloc(sizeof(char) * (strlen(move_f) + strlen(target_dir_path) + 1));
    sprintf(move_objs_command, move_f, target_dir_path);
    system(move_objs_command);
    free(move_objs_command);

    // Link the objs
    char *link_f = "gcc -B gcc $(find %s/obj -name \"*.o\") -o %s/bin/%s-%s";
    char *link_objs_command = malloc(sizeof(char) * (strlen(link_f) + strlen(target_dir_path) * 2 + strlen(package->name) + strlen(package->version) + 1));
    sprintf(link_objs_command, link_f, target_dir_path, target_dir_path, package->name, package->version);
    system(link_objs_command);
    free(link_objs_command);

    free(target_dir_path);
}
