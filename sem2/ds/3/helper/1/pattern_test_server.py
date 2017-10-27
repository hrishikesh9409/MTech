# Pattern RPC server


# Standard library imports
import concurrent
import json
import os
import sys
import time

# Imports from third party packages
import grpc

# Imports from current project
import pattern_pb2


class ChallengeServer(pattern_pb2.PatternGenTestServicer):
    def __init__(self):
        super(ChallengeServer, self).__init__()
        self.current_count = 0
        self.success_msg = "Yaay! All challenges solved!"
        self.trial_count = 50
        self.welcome = pattern_pb2.Welcome(
                        banner="Welcome to challenge.",
                        trial_count=self.trial_count
                       )
        return

    def _generate_challenge(self):
        curr_challenge = test_cases[self.current_count - 1]
        start = curr_challenge["start"]
        end = curr_challenge["end"]
        period = curr_challenge["period"]
        self.exp_ans = curr_challenge["ans"]
        self.challenge = pattern_pb2.Challenge(start=start, end=end, step=period)
        return

    def GetChallenge(self, request, context):
        if self.current_count == 0:
            # First request
            self.current_count += 1
            self._generate_challenge()
            response = pattern_pb2.ChallengeResponse(
                        challenge=self.challenge,
                        welcome=self.welcome
                       )
        elif request.solution == self.exp_ans:
            # Correct answer. Increment trial count. If done, stop else send next
            # challenge.
            self.current_count += 1
            if self.current_count > self.trial_count:
                response = pattern_pb2.ChallengeResponse(
                            correct=True,
                            status_msg=self.success_msg
                           )
                # Simulates disconnection since server thinks it's first request from
                # client.
                self.current_count = 0
            else:
                self._generate_challenge()
                response = pattern_pb2.ChallengeResponse(
                            challenge=self.challenge,
                            correct=True
                           )
        else:
            # Incorrect answer
            msg = "Incorrect answer! Got %s, expected %s" % \
                (request.solution, self.exp_ans)
            response = pattern_pb2.ChallengeResponse(
                        correct=False,
                        status_msg=msg
                       )
            # Simulates disconnection since server things it's first request from
            # client.
            self.current_count = 0

        return response


def serve():
    if len(sys.argv) != 3:
        print("Usage: %s PORT TEST_CASE_FILE" % (sys.argv[0]))
        sys.exit(1)

    port = int(sys.argv[1])
    if port < 1 or port >= pow(2, 16):
        print("Port number should lie between 1 and 65535")
        sys.exit(1)

    filename = sys.argv[2]
    if not os.path.isfile(filename):
        print("%s is not a valid file.")
        sys.exit(1)

    fp = open(filename)
    global test_cases
    try:
        test_cases = json.load(fp)
    except ValueError:
        print(("%s does not have valid JSON content. Please check if you chose the" +
              "correct file") % (filename))
        fp.close()
        sys.exit(1)

    fp.close()
    server = grpc.server(concurrent.futures.ThreadPoolExecutor(max_workers=10))
    pattern_pb2.add_PatternGenTestServicer_to_server(ChallengeServer(), server)
    server.add_insecure_port('[::]:%d' % (port))
    server.start()
    try:
        while True:
            time.sleep(60 * 60 * 24)
    except KeyboardInterrupt:
        server.stop(0)

    return


if __name__ == "__main__":
    serve()
