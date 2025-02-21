#ifndef GAME_ENGINE_PRIVATE_H
# define GAME_ENGINE_PRIVATE_H

# include "GE.h"
# include "GEPDebug.h"

# define ASSERT(_c, ...) if (!(_c)) { fprintf(stderr, __VA_ARGS__ ); exit(1); }
# define DEBUG_LOG(...) (void)fprintf(stderr, __VA_ARGS__ )

#endif