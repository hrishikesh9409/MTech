# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: pattern.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='pattern.proto',
  package='',
  syntax='proto3',
  serialized_pb=_b('\n\rpattern.proto\"$\n\x10\x43hallengeRequest\x12\x10\n\x08solution\x18\x01 \x01(\t\"5\n\tChallenge\x12\r\n\x05start\x18\x02 \x01(\t\x12\x0b\n\x03\x65nd\x18\x03 \x01(\t\x12\x0c\n\x04step\x18\x04 \x01(\x05\".\n\x07Welcome\x12\x0e\n\x06\x62\x61nner\x18\x01 \x01(\t\x12\x13\n\x0btrial_count\x18\x02 \x01(\x05\"\x88\x01\n\x11\x43hallengeResponse\x12\x0f\n\x07\x63orrect\x18\x01 \x01(\x08\x12\x14\n\nstatus_msg\x18\x02 \x01(\tH\x00\x12\x1f\n\tchallenge\x18\x03 \x01(\x0b\x32\n.ChallengeH\x00\x12\x19\n\x07welcome\x18\x04 \x01(\x0b\x32\x08.WelcomeB\x10\n\x0eresponse_oneof2I\n\x0ePatternGenTest\x12\x37\n\x0cGetChallenge\x12\x11.ChallengeRequest\x1a\x12.ChallengeResponse\"\x00\x62\x06proto3')
)
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_CHALLENGEREQUEST = _descriptor.Descriptor(
  name='ChallengeRequest',
  full_name='ChallengeRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='solution', full_name='ChallengeRequest.solution', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=17,
  serialized_end=53,
)


_CHALLENGE = _descriptor.Descriptor(
  name='Challenge',
  full_name='Challenge',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='start', full_name='Challenge.start', index=0,
      number=2, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='end', full_name='Challenge.end', index=1,
      number=3, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='step', full_name='Challenge.step', index=2,
      number=4, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=55,
  serialized_end=108,
)


_WELCOME = _descriptor.Descriptor(
  name='Welcome',
  full_name='Welcome',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='banner', full_name='Welcome.banner', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='trial_count', full_name='Welcome.trial_count', index=1,
      number=2, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=110,
  serialized_end=156,
)


_CHALLENGERESPONSE = _descriptor.Descriptor(
  name='ChallengeResponse',
  full_name='ChallengeResponse',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='correct', full_name='ChallengeResponse.correct', index=0,
      number=1, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='status_msg', full_name='ChallengeResponse.status_msg', index=1,
      number=2, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='challenge', full_name='ChallengeResponse.challenge', index=2,
      number=3, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='welcome', full_name='ChallengeResponse.welcome', index=3,
      number=4, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
    _descriptor.OneofDescriptor(
      name='response_oneof', full_name='ChallengeResponse.response_oneof',
      index=0, containing_type=None, fields=[]),
  ],
  serialized_start=159,
  serialized_end=295,
)

_CHALLENGERESPONSE.fields_by_name['challenge'].message_type = _CHALLENGE
_CHALLENGERESPONSE.fields_by_name['welcome'].message_type = _WELCOME
_CHALLENGERESPONSE.oneofs_by_name['response_oneof'].fields.append(
  _CHALLENGERESPONSE.fields_by_name['status_msg'])
_CHALLENGERESPONSE.fields_by_name['status_msg'].containing_oneof = _CHALLENGERESPONSE.oneofs_by_name['response_oneof']
_CHALLENGERESPONSE.oneofs_by_name['response_oneof'].fields.append(
  _CHALLENGERESPONSE.fields_by_name['challenge'])
_CHALLENGERESPONSE.fields_by_name['challenge'].containing_oneof = _CHALLENGERESPONSE.oneofs_by_name['response_oneof']
DESCRIPTOR.message_types_by_name['ChallengeRequest'] = _CHALLENGEREQUEST
DESCRIPTOR.message_types_by_name['Challenge'] = _CHALLENGE
DESCRIPTOR.message_types_by_name['Welcome'] = _WELCOME
DESCRIPTOR.message_types_by_name['ChallengeResponse'] = _CHALLENGERESPONSE

ChallengeRequest = _reflection.GeneratedProtocolMessageType('ChallengeRequest', (_message.Message,), dict(
  DESCRIPTOR = _CHALLENGEREQUEST,
  __module__ = 'pattern_pb2'
  # @@protoc_insertion_point(class_scope:ChallengeRequest)
  ))
_sym_db.RegisterMessage(ChallengeRequest)

Challenge = _reflection.GeneratedProtocolMessageType('Challenge', (_message.Message,), dict(
  DESCRIPTOR = _CHALLENGE,
  __module__ = 'pattern_pb2'
  # @@protoc_insertion_point(class_scope:Challenge)
  ))
_sym_db.RegisterMessage(Challenge)

Welcome = _reflection.GeneratedProtocolMessageType('Welcome', (_message.Message,), dict(
  DESCRIPTOR = _WELCOME,
  __module__ = 'pattern_pb2'
  # @@protoc_insertion_point(class_scope:Welcome)
  ))
_sym_db.RegisterMessage(Welcome)

ChallengeResponse = _reflection.GeneratedProtocolMessageType('ChallengeResponse', (_message.Message,), dict(
  DESCRIPTOR = _CHALLENGERESPONSE,
  __module__ = 'pattern_pb2'
  # @@protoc_insertion_point(class_scope:ChallengeResponse)
  ))
_sym_db.RegisterMessage(ChallengeResponse)


try:
  # THESE ELEMENTS WILL BE DEPRECATED.
  # Please use the generated *_pb2_grpc.py files instead.
  import grpc
  from grpc.framework.common import cardinality
  from grpc.framework.interfaces.face import utilities as face_utilities
  from grpc.beta import implementations as beta_implementations
  from grpc.beta import interfaces as beta_interfaces


  class PatternGenTestStub(object):

    def __init__(self, channel):
      """Constructor.

      Args:
        channel: A grpc.Channel.
      """
      self.GetChallenge = channel.unary_unary(
          '/PatternGenTest/GetChallenge',
          request_serializer=ChallengeRequest.SerializeToString,
          response_deserializer=ChallengeResponse.FromString,
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
            request_deserializer=ChallengeRequest.FromString,
            response_serializer=ChallengeResponse.SerializeToString,
        ),
    }
    generic_handler = grpc.method_handlers_generic_handler(
        'PatternGenTest', rpc_method_handlers)
    server.add_generic_rpc_handlers((generic_handler,))


  class BetaPatternGenTestServicer(object):
    """The Beta API is deprecated for 0.15.0 and later.

    It is recommended to use the GA API (classes and functions in this
    file not marked beta) for all further purposes. This class was generated
    only to ease transition from grpcio<0.15.0 to grpcio>=0.15.0."""
    def GetChallenge(self, request, context):
      context.code(beta_interfaces.StatusCode.UNIMPLEMENTED)


  class BetaPatternGenTestStub(object):
    """The Beta API is deprecated for 0.15.0 and later.

    It is recommended to use the GA API (classes and functions in this
    file not marked beta) for all further purposes. This class was generated
    only to ease transition from grpcio<0.15.0 to grpcio>=0.15.0."""
    def GetChallenge(self, request, timeout, metadata=None, with_call=False, protocol_options=None):
      raise NotImplementedError()
    GetChallenge.future = None


  def beta_create_PatternGenTest_server(servicer, pool=None, pool_size=None, default_timeout=None, maximum_timeout=None):
    """The Beta API is deprecated for 0.15.0 and later.

    It is recommended to use the GA API (classes and functions in this
    file not marked beta) for all further purposes. This function was
    generated only to ease transition from grpcio<0.15.0 to grpcio>=0.15.0"""
    request_deserializers = {
      ('PatternGenTest', 'GetChallenge'): ChallengeRequest.FromString,
    }
    response_serializers = {
      ('PatternGenTest', 'GetChallenge'): ChallengeResponse.SerializeToString,
    }
    method_implementations = {
      ('PatternGenTest', 'GetChallenge'): face_utilities.unary_unary_inline(servicer.GetChallenge),
    }
    server_options = beta_implementations.server_options(request_deserializers=request_deserializers, response_serializers=response_serializers, thread_pool=pool, thread_pool_size=pool_size, default_timeout=default_timeout, maximum_timeout=maximum_timeout)
    return beta_implementations.server(method_implementations, options=server_options)


  def beta_create_PatternGenTest_stub(channel, host=None, metadata_transformer=None, pool=None, pool_size=None):
    """The Beta API is deprecated for 0.15.0 and later.

    It is recommended to use the GA API (classes and functions in this
    file not marked beta) for all further purposes. This function was
    generated only to ease transition from grpcio<0.15.0 to grpcio>=0.15.0"""
    request_serializers = {
      ('PatternGenTest', 'GetChallenge'): ChallengeRequest.SerializeToString,
    }
    response_deserializers = {
      ('PatternGenTest', 'GetChallenge'): ChallengeResponse.FromString,
    }
    cardinalities = {
      'GetChallenge': cardinality.Cardinality.UNARY_UNARY,
    }
    stub_options = beta_implementations.stub_options(host=host, metadata_transformer=metadata_transformer, request_serializers=request_serializers, response_deserializers=response_deserializers, thread_pool=pool, thread_pool_size=pool_size)
    return beta_implementations.dynamic_stub(channel, 'PatternGenTest', cardinalities, options=stub_options)
except ImportError:
  pass
# @@protoc_insertion_point(module_scope)