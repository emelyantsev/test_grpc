#pragma once

#include <memory>
#include <string>
#include <vector>

#include <grpcpp/grpcpp.h>

#include "generated/geo.pb.h"
#include "generated/geo.grpc.pb.h"


#include "common.h"



class GeoClient {

    public:

        GeoClient(std::shared_ptr<::grpc::Channel> channel) ;

        bool AddCity(const GeoHelper::CityInfo& cityInfo) ;
        bool RemoveCity(const GeoHelper::CityInfo& cityInfo) ;

        std::vector<bool> AddCities(const std::vector<GeoHelper::CityInfo>& cities) ;


        std::string GetCityByCoords(const GeoHelper::Point& point) ;

        std::vector<GeoHelper::Point> GetCityByName(const std::string& cityName) ;

        std::vector<GeoHelper::CityInfo> GetCities() ;

    private:

        std::unique_ptr<Geo::GeoManager::Stub> stub_;

};
