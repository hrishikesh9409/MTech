#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Standard library imports
import json
import socket
import sys
import time


def main():
    if len(sys.argv) != 4:
        print("Usage: %s HOST PORT TESTS_FILE" % (sys.argv[0]))
        sys.exit(1)

    host = sys.argv[1]
    port = int(sys.argv[2])
    sample_tests_file = sys.argv[3]
    delimiter = "\r\n"
    timeout = 5

    fh = open(sample_tests_file)
    sample_tests = json.load(fh)
    fh.close()

    delimiter = "\n"
    timeout = 5
    trial_count = len(sample_tests)
    socket.setdefaulttimeout(timeout)
    curr_trial = 1
    success = True

    s = socket.create_connection((host, port))
    s.send("Total trial count: %d, timeout: %ds" % (trial_count, timeout) + delimiter)
    time.sleep(1)
    for test_case in sample_tests:
        n = test_case
        solution = sample_tests[test_case]
        s.send(str(n) + delimiter)
        time.sleep(1)
        try:
            response = long(s.makefile().readline().strip())
        except socket.timeout:
            s.send("Boo! Too slow! Bye!" + delimiter)
            success = False
            break
        except ValueError:
            response = None

        if response != solution:
            s.send("Got %s, expected %d" % (str(response), solution) + delimiter)
            success = False
            break
        else:
            curr_trial += 1

    if success:
        print("Success")
        s.send("0" + delimiter)
    else:
        print("Failed")

    s.close()
    return


if __name__ == "__main__":
    main()
