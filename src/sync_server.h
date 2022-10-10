#pragma once

#include <grpcpp/grpcpp.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/health_check_service_interface.h>

#include "generated/geo.pb.h"
#include "generated/geo.grpc.pb.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <shared_mutex>

#include "common.h"

class GeoManagerImpl final : public Geo::GeoManager::Service {

    public:

        ::grpc::Status AddCity(::grpc::ServerContext* context, const ::Geo::CityInfo* request, ::Geo::StatusResponse* response) override;
        ::grpc::Status RemoveCity(::grpc::ServerContext* context, const ::Geo::CityInfo* request, ::Geo::StatusResponse* response) override;
        ::grpc::Status AddCities(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::Geo::StatusResponse, ::Geo::CityInfo>* stream) override;
        ::grpc::Status GetCityByName(::grpc::ServerContext* context, const ::Geo::CityName* request, ::grpc::ServerWriter<::Geo::Point>* writer) override;
        ::grpc::Status GetCityByCoords(::grpc::ServerContext* context, const ::Geo::Point* request, ::Geo::CityName* response) override;
        ::grpc::Status GetCities(::grpc::ServerContext* context, const ::Geo::Void* request, ::grpc::ServerWriter< ::Geo::CityInfo>* writer) override;


    private:

        bool addCity(const GeoHelper::CityInfo& cityInfo) ;
        bool removeCity(const GeoHelper::CityInfo& cityInfo) ;

        std::unordered_map<GeoHelper::Point, std::string> pointsNames;
        std::unordered_map<std::string, std::unordered_set<GeoHelper::Point> > namesPoints;

        mutable std::shared_mutex mtx;
} ;

