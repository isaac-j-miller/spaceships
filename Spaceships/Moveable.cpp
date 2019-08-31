#include "Moveable.h"


point Moveable::getTrajectory() {
	return getComponents(rotation * M_PI + M_PI_2);
}