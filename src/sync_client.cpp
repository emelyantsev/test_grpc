#include "sync_client.h"

GeoClient::GeoClient(std::shared_ptr<::grpc::Channel> channel) : 
    stub_(Geo::GeoManager::NewStub(channel) ) {}


bool GeoClient::AddCity(const GeoHelper::CityInfo& cityInfo) {

    Geo::CityInfo request;

    request.set_name(cityInfo.name) ;

    Geo::Point cityLocation;

    request.mutable_location()->set_latitude( cityInfo.point.latitude ) ;
    request.mutable_location()->set_longitude( cityInfo.point.longitude ) ;


    Geo::StatusResponse response;
    ::grpc::ClientContext context;


    ::grpc::Status status = stub_->AddCity(&context, request, &response) ;

    if ( status.ok() ) {

        if (response.status() == Geo::Status::STATUS_OK) {

            std::cout << "Add city " << cityInfo.name << std::endl; 
            return true;
        }
        else {

            std::cout << "Error adding city " << cityInfo.name << std::endl; 
            return false;
        }
    } 
    else {

      std::cout << status.error_code() << ": " << status.error_message() << std::endl;
      
      return false;
    }

}

std::vector<bool> GeoClient::AddCities(const std::vector<GeoHelper::CityInfo>& cities) {

    ::grpc::ClientContext context;

    std::unique_ptr< ::grpc::ClientReaderWriter< ::Geo::CityInfo, ::Geo::StatusResponse > > stream(
        stub_->AddCities(&context) );


    std::vector<bool> result;

    for (const auto& city : cities) {

        Geo::CityInfo request;

        request.set_name(city.name) ;

        Geo::Point cityLocation;

        request.mutable_location()->set_latitude( city.point.latitude ) ;
        request.mutable_location()->set_longitude( city.point.longitude ) ;

        stream->Write(request) ;

        Geo::StatusResponse response;

        bool readResult = stream->Read(&response) ;


        if (readResult == false) {
            break;
        }

        if (response.status() == Geo::Status::STATUS_OK) {

            std::cout << "Add city " << city.name << std::endl; 
            result.push_back(true) ;
        }
        else {

            std::cout << "Error adding city " << city.name << std::endl; 
            result.push_back(false) ;
        }
    }

    stream->WritesDone();    

    ::grpc::Status status = stream->Finish();
    
    if (!status.ok()) {

      std::cout << "Add cities rpc failed." << std::endl;
    }

    return result;
}


bool GeoClient::RemoveCity(const GeoHelper::CityInfo& cityInfo) {

    ::Geo::CityInfo request;

    request.set_name(cityInfo.name);

    ::Geo::Point cityLocation;

    request.mutable_location()->set_latitude( cityInfo.point.latitude ) ;
    request.mutable_location()->set_longitude( cityInfo.point.longitude ) ;

    ::Geo::StatusResponse response;
    ::grpc::ClientContext context;

    ::grpc::Status status = stub_->RemoveCity(&context, request, &response) ;

    if ( status.ok() ) {

        if (response.status() == ::Geo::Status::STATUS_OK) {

            std::cout << "Remove city " << cityInfo.name << std::endl; 
            return true;
        }
        else {

            std::cout << "Error removing city " << cityInfo.name << std::endl; 
            return false;
        }
    } 
    else {

      std::cout << status.error_code() << ": " << status.error_message() << std::endl; 

      return false;
    }
}


std::vector<GeoHelper::CityInfo> GeoClient::GetCities() {


    Geo::Void request;

    ::grpc::ClientContext context;

    std::unique_ptr<::grpc::ClientReader<Geo::CityInfo> > reader(stub_->GetCities( &context, request) ) ;

    std::vector<GeoHelper::CityInfo> result;


    Geo::CityInfo cityInfo;

    while ( reader->Read(&cityInfo) )  {

        GeoHelper::CityInfo newInfo;

        newInfo.name = cityInfo.name();
        newInfo.point.latitude = cityInfo.location().latitude();
        newInfo.point.longitude = cityInfo.location().longitude(); 

        result.push_back(newInfo) ;
    }

    ::grpc::Status status = reader->Finish();

    return result;
}


std::string GeoClient::GetCityByCoords(const GeoHelper::Point& point) {

    ::grpc::ClientContext context;

    Geo::Point pointRequest; 
    pointRequest.set_latitude(point.latitude);
    pointRequest.set_longitude(point.longitude);

    Geo::CityName nameResponse;

    ::grpc::Status status = stub_->GetCityByCoords(&context, pointRequest, &nameResponse) ;


    if ( status.ok() ) {

        return nameResponse.name();
    } 
    else {

      std::cout << status.error_code() << ": " << status.error_message() << std::endl;
      
      return {};
    }
}


std::vector<GeoHelper::Point> GeoClient::GetCityByName(const std::string& cityName) {

    ::Geo::CityName request;

    request.set_name(cityName) ;

    ::grpc::ClientContext context;

    std::unique_ptr<::grpc::ClientReader<::Geo::Point> > reader(stub_->GetCityByName( &context, request) ) ;

    std::vector<GeoHelper::Point> result;

    ::Geo::Point point;

    while ( reader->Read(&point) )  {

        GeoHelper::Point newPoint;

        newPoint.latitude = point.latitude();
        newPoint.longitude = point.longitude(); 

        result.push_back(newPoint) ;
    }

    ::grpc::Status status = reader->Finish();

    return result;

}