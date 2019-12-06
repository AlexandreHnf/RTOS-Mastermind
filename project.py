from itertools import product
from threading import Thread
import random
import time
import sys

# Arguments : COLORS SPOTS NB_NODES [PRINT]
COLORS = int(sys.argv[1])
SPOTS = int(sys.argv[2])
NB_NODES = int(sys.argv[3])  # = number of threads
PRINT = False                # Show print's or not
if len(sys.argv) >= 5:
    PRINT = True

exitApp = False  # Allows CTRL-C to stop the threads

class Node(Thread):
    def __init__(self, nodeID):
        Thread.__init__(self)
        self.id = nodeID
        self.secret = 0
        self.combis = []
    
    def addCombi(self, combi):
        self.combis.append(combi)
    
    def setSecret(self, secret):
        self.secret = secret

    def run(self):
        while(len(self.combis) > 1) and not exitApp:
            test = random.choice(self.combis)
            scoreTest = score(self.secret, test)
            for combi in self.combis:
                if score(combi, test) != scoreTest:
                    self.combis.remove(combi)
            if PRINT:
                print("Node", self.id, ":", len(self.combis), "combinations left")
        if self.combis[0] != self.secret:
            self.combis.remove(self.combis[0])

def main():
    timeStart = time.time()
    combis = product(range(COLORS), repeat=SPOTS)
    nbCombis = COLORS**SPOTS
    print(nbCombis, "possible combinations")

    nodes = []
    for i in range(NB_NODES):
        nodes.append(Node(i))
    
    i = 0
    for combi in combis:
        nodes[i % NB_NODES].addCombi(combi)
        i += 1
    
    secret = random.choice(random.choice(nodes).combis)
    print("Secret :", secret)
    for node in nodes:
        node.setSecret(secret)
        node.daemon = True
        node.start()
    for node in nodes:
        node.join()
    solution = None
    for node in nodes:
        if len(node.combis) == 1:
            solution = node.combis[0]
            break
    print("Solution : " + str(solution), "in " + str(time.time() - timeStart), "seconds")

def score(a, b):
    correct, present = 0, 0
    for i, j in zip(a, b):
        if i == j:
            correct += 1
    present = len(set(a).intersection(set(b))) - correct
    return correct, present

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:   # Handle CTRL-C
        exitApp = True
        print("STOP")
