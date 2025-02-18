#ifndef TEST_H
# define TEST_H

# include <GE.h>

typedef enum {
    OFFSET_CMP = DEFAULT_COMPONENT_COUNT,
    POSITION_CMP,
    VELOCITY_CMP,
}   ComponentLocal;

typedef enum {
    OFFSET_EVTP = DEFAULT_EVENT_TYPE_COUNT,
}   EventTypeLocal;

#endif