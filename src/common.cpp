#include "common.h"

bool GeoHelper::Point::operator==(const GeoHelper::Point& other) const {

    return latitude == other.latitude && longitude == other.longitude;
}