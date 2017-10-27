#! /usr/bin/python

import test_pb2
import sys

# Iterates though all people in the test and prints info about them.
def ListPeople(address_book):
  for person in address_book.person:
    print "Person ID:", person.id
    print "  Name:", person.name
    if person.HasField('email'):
      print "  E-mail address:", person.email

    for phone_number in person.phones:
      if phone_number.type == test_pb2.Person.MOBILE:
        print "  Mobile phone #: ",
      elif phone_number.type == test_pb2.Person.HOME:
        print "  Home phone #: ",
      elif phone_number.type == test_pb2.Person.WORK:
        print "  Work phone #: ",
      print phone_number.number

# Main procedure:  Reads the entire address book from a file and prints all
#   the information inside.
if len(sys.argv) != 2:
  print "Usage:", sys.argv[0], "ADDRESS_BOOK_FILE"
  sys.exit(-1)

address_book = test_pb2.test()

# Read the existing address book.
f = open(sys.argv[1], "rb")
address_book.ParseFromString(f.read())
f.close()

ListPeople(address_book)