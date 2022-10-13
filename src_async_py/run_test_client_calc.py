from urllib import response
import grpc

import calculator_pb2
import calculator_pb2_grpc

import random
import sys
import argparse

numbers = []

seed = 845

def addNumbers(stub):

    random.seed(seed)

    for i in range(100):

        x = random.randint(0, 1000000)
        y = random.randint(0, 1000000)

        response = stub.AddNumbers(calculator_pb2.NumbersRequest(x=x, y=y))

        #print(x, y, response.value)

        assert response.value == x + y


def multiplyNumbers(stub : calculator_pb2_grpc.CalculatorStub):

    random.seed(seed)

    for i in range(100):

        x = random.randint(0, 1000)
        y = random.randint(0, 1000)

        response = stub.MultiplyNumbers(calculator_pb2.NumbersRequest(x=x, y=y))

        # print(x, y, response.value)

        assert response.value == x * y


def generateNumbers():

    random.seed(seed)

    global numbers
    numbers.clear()

    for i in range(9):

        number = random.randint(0, 10)
        numbers.append(number)

    for number in numbers:
        yield calculator_pb2.NumberRequest(value=number)

def sumNumbers(stub : calculator_pb2_grpc.CalculatorStub):

    result = stub.SumNumbers(generateNumbers())

    # print(result.value)

    assert result.value == sum( numbers )



if __name__ == '__main__':

    parser = argparse.ArgumentParser()

    parser.add_argument('-s', '--seed', type=int, help="sets random seed")

    args = parser.parse_args()

    if args.seed:
        seed = args.seed

    

    print(f"Client starts working with seeed = {seed}...")

    with grpc.insecure_channel('localhost:50051') as channel:
        
        stub = calculator_pb2_grpc.CalculatorStub(channel)
        
        addNumbers(stub)
        multiplyNumbers(stub)
        sumNumbers(stub)
    
    print("Client finished OK")
    
    sys.exit(0)