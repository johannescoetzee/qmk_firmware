#pragma once
#include <stdlib.h>

#define GROUP_COUNT 3
#define GROUP_SIZE 7

enum layers {
    DVK = 0,
    GDV,
    RGB,
    GDM,
    GDN,
    NAV,
    NUM,
    LSM,
    RSM,
    FUN,
    CTL,
    LAST
};

typedef struct layer_group {
    char *name;
    int layers[GROUP_SIZE];
} LayerGroup;

typedef struct {
    int current;
    int size;
    LayerGroup groups[GROUP_COUNT];
} GroupRingBuffer;

