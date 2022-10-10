#include "sync_client.h"

#include <string>
#include <sstream>

int main(int argc, char* argv[] ) {

    std::string addressStr = "localhost";
    std::string portStr = "50051";

    if (argc > 1) {

        addressStr = argv[1] ;
    }

    if (argc > 2) {

        portStr = argv[2] ;
    }

    std::string target_str = addressStr + ":" + portStr; 

    GeoClient client( ::grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials() ) ) ;

    std::string lineStr;

    std::cout << "Supported commands: ADD, ADDMANY, REMOVE, GETALL, GETBYCOORDS, GETBYNAME" << std::endl;

    while ( std::getline(std::cin, lineStr) ) {

        std::stringstream ss(lineStr);

        std::string command;
        ss >> command;

        if (command == "ADD") {

            GeoHelper::CityInfo cityInfo;

            ss >> cityInfo.name >> cityInfo.point.latitude >> cityInfo.point.longitude;

            bool result = client.AddCity(cityInfo) ;

            std::cout << std::boolalpha << result << std::endl;
        }

        else if (command == "REMOVE") {

            GeoHelper::CityInfo cityInfo;
            
            ss >> cityInfo.name >> cityInfo.point.latitude >> cityInfo.point.longitude;

            bool result = client.RemoveCity(cityInfo) ;

            std::cout << std::boolalpha << result << std::endl;

        }

        else if (command == "GETALL" ) {

            auto result = client.GetCities() ;

            for (const auto& item : result) {

                std::cout << item.name << " " << item.point.latitude << " : " << item.point.longitude << std::endl;
            }
        }

        else if (command == "GETBYCOORDS") {


            GeoHelper::Point point;
            
            ss >> point.latitude >> point.longitude;

            auto result = client.GetCityByCoords(point) ;

            if (!result.empty()) {

                std::cout << "Found city " << result << " at " << point.latitude << ":" << point.longitude << std::endl;
            }
            else {

                std::cout << "City not found" << std::endl;
            }

        }
        else if (command == "GETBYNAME") {


            std::string cityName;
            ss >> cityName;

            auto result = client.GetCityByName(cityName) ;

            for (const auto& point : result) {

                std::cout << "Found city " << cityName << " at " << point.latitude << " : " << point.longitude << std::endl;
            }


        }

        else if (command == "ADDMANY" ) {

            int count;
            ss >> count;

            std::vector<GeoHelper::CityInfo> input;
            input.reserve(count);

            for (int i = 0; i < count; ++i) {

                GeoHelper::CityInfo cityInfo;

                ss >> cityInfo.name >> cityInfo.point.latitude >> cityInfo.point.longitude ;

                input.push_back(cityInfo);
            }

            auto result = client.AddCities(input);

            for (int i = 0; i < result.size() ; ++i) {

                std::cout << std::boolalpha << result[i] << std::endl;
            }
        }

        else {

            std::cout << "Unknown command" << std::endl;
        }
    }

    return 0;
}