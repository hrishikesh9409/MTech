#Program by Hrishikesh N - AM.EN.P2CSN16008
#7 - 2 - 2017
#program to compute the letter in between given start , end and period using grpc

from __future__ import print_function

import grpc

import pattern_pb2
import pattern_pb2_grpc
import sys

def fun(start, end, period):
	i = 0
	answer = " ".join(chr(i) for i in range(ord(start), ord(end)+1,int(period)))
	return answer	


def run():
  channel = grpc.insecure_channel(sys.argv[1] + ':' + sys.argv[2])
  stub = pattern_pb2_grpc.PatternGenTestStub(channel)
  i = 1
  response = stub.GetChallenge(pattern_pb2.ChallengeRequest(solution=''))
  while(i <= 50):
    start = response.challenge.start
    end = response.challenge.end
    period = response.challenge.step
    answer = fun(start, end, period)
    print (answer)
    
    response = stub.GetChallenge(pattern_pb2.ChallengeRequest(solution=answer))
    i += 1	


if __name__ == '__main__':
  run()