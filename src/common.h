#pragma once

#include <string>
#include <functional>

namespace GeoHelper {


    struct Point {
        int latitude;
        int longitude;

        bool operator==(const GeoHelper::Point& other) const;
    };

    struct CityInfo {
        
        Point point;
        std::string name;
    };

}


template<>
struct std::hash<GeoHelper::Point> {

    std::size_t operator()(GeoHelper::Point const& point) const noexcept {

        long long result = ( static_cast<long long>(point.latitude) << 32 ) | point.longitude ;
        return std::hash<long long>{}(result) ;
    }

};

