#include "sync_server.h"

#include <iostream>

::grpc::Status GeoManagerImpl::AddCity(::grpc::ServerContext* context, const ::Geo::CityInfo* request, ::Geo::StatusResponse* response) {


    GeoHelper::CityInfo cityInfo;

    cityInfo.point = { request->location().latitude(), request->location().longitude() } ;
    cityInfo.name = request->name();

    bool result = addCity(cityInfo);

    if (result) {
    
        response->set_status(::Geo::Status::STATUS_OK) ;
    }
    else {

        response->set_status(::Geo::Status::STATUS_ERROR) ;
    }

    return ::grpc::Status::OK;
}

::grpc::Status GeoManagerImpl::AddCities(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::Geo::StatusResponse, ::Geo::CityInfo>* stream) {

    Geo::CityInfo newInfo;

    while ( stream->Read(&newInfo ) ) {

        GeoHelper::CityInfo cityInfo;

        cityInfo.name = newInfo.name();
        cityInfo.point.latitude = newInfo.location().latitude();
        cityInfo.point.longitude = newInfo.location().longitude();

        bool result = addCity(cityInfo);

        ::Geo::StatusResponse response;

        if (result) {
        
            response.set_status(::Geo::Status::STATUS_OK) ;
        }
        else {
            response.set_status(::Geo::Status::STATUS_ERROR) ;
        }

        stream->Write( response ) ;
    }

    return ::grpc::Status::OK;   
}

::grpc::Status GeoManagerImpl::GetCityByName(::grpc::ServerContext* context, const ::Geo::CityName* request, ::grpc::ServerWriter<::Geo::Point>* writer) {


    std::shared_lock<std::shared_mutex> lock(mtx);

    if (namesPoints.count( request->name() ) != 0 ) {

        std::vector<::Geo::Point> result;

        result.reserve( namesPoints[ request->name() ].size()  ) ;

        for (const auto& point : namesPoints[ request->name() ] ) {

            
            ::Geo::Point pointToSend;

            pointToSend.set_latitude(point.latitude) ;
            pointToSend.set_longitude(point.longitude) ;

            result.push_back(pointToSend) ;
        }

        for (const auto& item : result) {

            writer->Write(item) ;
        }
    }

    return ::grpc::Status::OK;
}

::grpc::Status GeoManagerImpl::GetCityByCoords(::grpc::ServerContext* context, const ::Geo::Point* request, ::Geo::CityName* response) {

    GeoHelper::Point point = { request->latitude(), request->longitude() } ;

    std::shared_lock<std::shared_mutex> lock;

    if ( pointsNames.count(point) ) {

        std::string cityName = pointsNames[point] ;

        *response->mutable_name() = cityName; 
        return ::grpc::Status::OK;
    }
    else {

        std::string error_msg = "The city with given coords is not found" ;

        return ::grpc::Status( grpc::StatusCode::NOT_FOUND, error_msg ) ; 
    }
    
}

::grpc::Status GeoManagerImpl::GetCities(::grpc::ServerContext* context, const ::Geo::Void* request, ::grpc::ServerWriter< ::Geo::CityInfo>* writer) {


    mtx.lock_shared();

    std::vector<Geo::CityInfo> result;

    result.reserve( pointsNames.size() ) ;

    for (const auto& item : pointsNames) {

        Geo::CityInfo cityInfo;

        cityInfo.set_name(item.second);
        cityInfo.mutable_location()->set_latitude(item.first.latitude);
        cityInfo.mutable_location()->set_longitude(item.first.longitude);

        result.push_back(cityInfo) ;
    }

    mtx.unlock();

    for (const auto& item : result) {

        writer->Write( item ) ;

    }

    return ::grpc::Status::OK;
}


bool GeoManagerImpl::addCity(const GeoHelper::CityInfo& cityInfo) {

    std::unique_lock<std::shared_mutex> lock(mtx);

    if ( pointsNames.count(cityInfo.point) == 1 )  {
      
        std::string cityName = pointsNames[cityInfo.point];

        if (cityName == cityInfo.name ) {

            std::cout << "Existing city " << cityName << " at " << cityInfo.point.latitude << ":" << cityInfo.point.longitude << std::endl;
            return false;
        }
        else {
            
            std::cout << "Another city " << cityName << "is found at " << cityInfo.point.latitude << ":" << cityInfo.point.longitude << std::endl;      
            return false;
        }
    }
    else {

        pointsNames[cityInfo.point] = cityInfo.name;
        namesPoints[cityInfo.name].insert(cityInfo.point);

        std::cout << "Added city " << cityInfo.name << " at " << cityInfo.point.latitude << ":" << cityInfo.point.longitude << std::endl;

        return true;
    }
}

::grpc::Status GeoManagerImpl::RemoveCity(::grpc::ServerContext* context, const ::Geo::CityInfo* request, ::Geo::StatusResponse* response) {

    GeoHelper::CityInfo cityInfo;

    cityInfo.point = { request->location().latitude(), request->location().longitude() } ;
    cityInfo.name = request->name();

    bool result = removeCity(cityInfo);

    if (result) {
    
        response->set_status(::Geo::Status::STATUS_OK) ;
    }
    else {

        response->set_status(::Geo::Status::STATUS_ERROR) ;
    }

    return ::grpc::Status::OK;
}



bool GeoManagerImpl::removeCity(const GeoHelper::CityInfo& cityInfo) {

    std::unique_lock<std::shared_mutex> lock(mtx);

    if ( pointsNames.count(cityInfo.point) == 0 ||  pointsNames[cityInfo.point] != cityInfo.name )  {


        std::cout << "Not found city " << cityInfo.name << " at " << cityInfo.point.latitude << ":" << cityInfo.point.longitude << std::endl;
      
        return false;
    }
    else {

        pointsNames.erase(cityInfo.point) ;

        namesPoints[cityInfo.name].erase(cityInfo.point) ;

        if (namesPoints[cityInfo.name].size() == 0) {

            namesPoints.erase( cityInfo.name ) ;
        }

        std::cout << "Remove city " << cityInfo.name << " at " << cityInfo.point.latitude << ":" << cityInfo.point.longitude << std::endl;

        return true;
    }

}

