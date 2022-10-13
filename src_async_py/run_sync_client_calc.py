import grpc

import calculator_pb2
import calculator_pb2_grpc


def addNumbers(stub):

    response = stub.AddNumbers(calculator_pb2.NumbersRequest(x=8, y=4))
    print(response.value)

def multiplyNumbers(stub):

    x = 8
    y = 4

    response = stub.MultiplyNumbers(calculator_pb2.NumbersRequest(x=x, y=y))
    print(response.value)
    assert response.value == x * y

def generateNumbers():
    numbers = [8,4,5,7,8]

    for number in numbers:
        yield calculator_pb2.NumberRequest(value=number)

def sumNumbers(stub : calculator_pb2_grpc.CalculatorStub):

    result = stub.SumNumbers(generateNumbers())

    print(result.value)

def run():

    print("Client starts working ...")

    with grpc.insecure_channel('localhost:50051') as channel:
        
        stub = calculator_pb2_grpc.CalculatorStub(channel)
        
        addNumbers(stub)
        multiplyNumbers(stub)
        sumNumbers(stub)
    
    print("Client finished OK")


if __name__ == '__main__':
    run()