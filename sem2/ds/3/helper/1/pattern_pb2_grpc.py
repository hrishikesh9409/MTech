# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
import grpc
from grpc.framework.common import cardinality
from grpc.framework.interfaces.face import utilities as face_utilities

import pattern_pb2 as pattern__pb2


class PatternGenTestStub(object):

  def __init__(self, channel):
    """Constructor.

    Args:
      channel: A grpc.Channel.
    """
    self.GetChallenge = channel.unary_unary(
        '/PatternGenTest/GetChallenge',
        request_serializer=pattern__pb2.ChallengeRequest.SerializeToString,
        response_deserializer=pattern__pb2.ChallengeResponse.FromString,
        )


class PatternGenTestServicer(object):

  def GetChallenge(self, request, context):
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')


def add_PatternGenTestServicer_to_server(servicer, server):
  rpc_method_handlers = {
      'GetChallenge': grpc.unary_unary_rpc_method_handler(
          servicer.GetChallenge,
          request_deserializer=pattern__pb2.ChallengeRequest.FromString,
          response_serializer=pattern__pb2.ChallengeResponse.SerializeToString,
      ),
  }
  generic_handler = grpc.method_handlers_generic_handler(
      'PatternGenTest', rpc_method_handlers)
  server.add_generic_rpc_handlers((generic_handler,))
