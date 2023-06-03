#pragma once
#include <stdlib.h>

enum layers {
    DVK = 0,
    GDV,
    GDM,
    GDN,
    NAV,
    NUM,
    SYM,
    FUN,
    LAST
};

const char* layer_names[] = {
    [DVK] = "DVK",
    [GDV] = "GDV",
    [GDN] = "GDN",
    [GDM] = "GDM",
    [NAV] = "NAV",
    [NUM] = "NUM",
    [SYM] = "SYM",
    [FUN] = "FUN",
    [LAST] = "LAST"
};

typedef struct layer_group {
    char *name;
    int *layers;
} LayerGroup;

typedef struct {
    int current;
    int size;
    LayerGroup *groups;
} GroupRingBuffer;

