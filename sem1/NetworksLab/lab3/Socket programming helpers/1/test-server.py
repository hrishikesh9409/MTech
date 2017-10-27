#!/usr/bin/env python


# Standard library imports
import json
import socket
import sys
import time


def main():
    if len(sys.argv) != 3:
        print("Usage: %s PORT TESTS_FILE" % (sys.argv[0]))
        sys.exit(0)

    delimiter = "\n"
    port = int(sys.argv[1])
    test_cases_file = sys.argv[2]
    challenge_message = "Start: %c, end: %c, period: %d" + delimiter
    challenge_wrong = "Wrong! Got %s, expected %s" + delimiter
    done = "Yaay! All trials done! Nice job" + delimiter
    server = ("0.0.0.0", port)
    success = True
    timeout = 3
    too_slow = "Boo! You're too slow! Bye!" + delimiter

    fh = open(test_cases_file)
    test_cases = json.load(fh)
    fh.close()
    max_trials = len(test_cases)
    welcome = "Welcome to challenge. Timeout: %ds, total trials: %d" % \
              (timeout, max_trials) + delimiter

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind(server)
    sock.listen(1)
    client_sock, client = sock.accept()
    client_sock.settimeout(timeout)

    try:
        client_sock.send(welcome)
        time.sleep(1)
        for challenge in test_cases:
            client_sock.send(challenge_message % (challenge['start'],
                                                  challenge['end'],
                                                  challenge['period']))
            time.sleep(1)
            answer = client_sock.makefile().readline().strip()
            if answer != challenge['ans']:
                client_sock.send(challenge_wrong % (answer, challenge['ans']))
                success = False
                break
    except socket.timeout:
        client_sock.send(too_slow)
        success = False

    if success:
        client_sock.send(done)

    client_sock.close()


if __name__ == "__main__":
    main()
