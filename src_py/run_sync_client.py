import grpc

import geo_pb2
import geo_pb2_grpc




def queryAllCities(stub):
      
    response = stub.GetCities(geo_pb2.Void())

    for item in response:
        print(item.name)
        print(item.location)

def addCity(stub):

    response = stub.AddCity(geo_pb2.CityInfo(name="Moscow", location=geo_pb2.Point(latitude=3, longitude=9)))
    print(response.status)


def removeCity(stub):

    response = stub.RemoveCity(geo_pb2.CityInfo(name="Moscow", location=geo_pb2.Point(latitude=3, longitude=9)))
    print(response.status)


def generateCityInfoList():

    cities = [ geo_pb2.CityInfo(name="Samara", location=geo_pb2.Point(latitude=23, longitude=19)) ,
            geo_pb2.CityInfo(name="Saratov", location=geo_pb2.Point(latitude=253, longitude=1)),
            geo_pb2.CityInfo(name="Sarov", location=geo_pb2.Point(latitude=2223, longitude=191))
    ]

    for city in cities:
        yield city
    
def addCities(stub):

    responses = stub.AddCities(generateCityInfoList() )

    for response in responses:
        print(response.status)

def getByName(stub):

    responses = stub.GetCityByName( geo_pb2.CityName(name="Moscow") )

    for response in responses:
        print(response)


def getByCoords(stub : geo_pb2_grpc.GeoManagerStub):

    response = stub.GetCityByCoords(geo_pb2.Point(latitude=23, longitude=19) )
    print(response.name)


def run():

    print("Client starts working ...")

    with grpc.insecure_channel('localhost:50051') as channel:
        
        stub = geo_pb2_grpc.GeoManagerStub(channel)

        queryAllCities(stub)
        addCity(stub)
        removeCity(stub)
        addCities(stub)
        getByName(stub)
        getByCoords(stub)
        
    
    print("Client finished OK")


if __name__ == '__main__':
    run()