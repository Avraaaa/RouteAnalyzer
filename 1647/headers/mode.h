#ifndef MODE_H
#define MODE_H

#define EARTH_RADIUS_KM 6371.0
#define PI 3.14159265358979323846

typedef enum {
    MODE_WALK,
    MODE_CAR,
    MODE_METRO,
    MODE_BIKOLPO,
    MODE_UTTARA
} Mode;

#define CAR_RATE 20.0
#define METRO_RATE 5.0
#define BIKOLPO_RATE 7.0
#define UTTARA_RATE 10.0

#define SPEED_CAR 30.0
#define SPEED_METRO 40.0
#define SPEED_BUS 25.0
#define SPEED_WALK 5.0

const char* getModeName(Mode mode);

#endif
