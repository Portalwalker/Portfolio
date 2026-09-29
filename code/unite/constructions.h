#ifndef CONSTRUCTION_H
#define CONSTRUCTION_H

	#include "dep/types.h"

	#define stackstruct(type, ...) cast(((type[1]){ [0]={ __VA_ARGS__ } }), type*)
	#define tempval(type, ...) cast(((type[1]){ [0]={ __VA_ARGS__ } }), type*)
	#define litlen(string_literal) (sizeof((string_literal)) - 1)

#endif // CONSTRUCTION_H
