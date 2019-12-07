from itertools import product
from threading import Thread
import random
import time
import sys

# Arguments : COLORS SPOTS NB_NODES [PRINT]
COLORS = int(sys.argv[1])
SPOTS = int(sys.argv[2])
NB_FIXED_SPOTS = int(sys.argv[3])  # = fixed spots
NB_NODES = COLORS**NB_FIXED_SPOTS   # 4spots, 2fixed => 4**2 nodes
PRINT = False                # Show print's or not
if len(sys.argv) >= 5:
	PRINT = True

exitApp = False  # Allows CTRL-C to stop the threads

class Node(Thread):
	def __init__(self, nodeID, fixed_spots):
		Thread.__init__(self)
		self.id = nodeID
		self.fixed_spots = fixed_spots
		self.prev_guesses = []  # à changer en dico
		self.prev_scores = []
		self.new_guess = None

	def run(self):
		combis = product(range(COLORS), repeat=(SPOTS-NB_FIXED_SPOTS))
		for combi in combis:
			combi = self.fixed_spots + combi # (0, 1) + (2, 3, 4) = (0, 1, 2, 3, 4)
			if plausible(combi, self.prev_guesses, self.prev_scores):
				print("Plausible guess :", combi)
				self.new_guess = combi
				return  # Don't go any further
			# else:
				# print("not plausible guess :", combi)

def score(a, b):
	correct, present = 0, 0
	for i, j in zip(a, b):
		if i == j:
			correct += 1
	present = len(set(a).intersection(set(b))) - correct
	return correct, present

def plausible(new_guess, prev_guesses, prev_scores):
	ok = True
	# ex : 1 2 3 4 (2, 1) sum = 3 good colors 
	#      1 2 3 5 = plausible since at least 3 same colors as the previous one
	#      1 2 4 6 = not plausible since just 2 colors are the same as the previous	
	if len(prev_guesses) > 0 and len(set(new_guess).intersection(set(prev_guesses[-1]))) < \
		prev_scores[-1][0] + prev_scores[-1][1]:
		return False
	
	for prev_guess, prev_score in zip(prev_guesses, prev_scores):
		# prev : 1 2 3 4 
		# new  : 1 2 3 4  Not ok, already said no...
		if new_guess == prev_guess:
			return False
		# prev : 1 2 3 3  score : (2, 0)
		# new  : 1 2 4 5  => len ( (1 2 3) U (1 2 4 5) ) = 2 ok     > (2, 0)
		# new  : 1 4 5 6  => len ( (1 2 3) U (1 4 5 6) ) = 1 not ok < (2, 0)
		if len(set(prev_guess).union(set(new_guess))) < prev_score[0]:
			return False
		# prev : 1 2 3 4  score : (0, 0)
		# new  : 2 3 4 1  => duhhh score (0, 0) aussi 
		if prev_score == (0, 0) and len(set(prev_guess).difference(set(new_guess)))== 0:
			return False
			
		
	return ok 
	
	
def masterNode():
	combis = product(range(COLORS), repeat=SPOTS)
	nbCombis = COLORS**SPOTS
	print(nbCombis, "possible combinations")

	secret = tuple([random.randint(0, COLORS-1) for x in range(SPOTS)])
	print("Secret : " + str(secret))

	prev_guesses = []
	prev_scores = []
	for i in range(NB_NODES):
		prev_guesses.append([])
		prev_scores.append([])
	
	solutionFound = None
	finished = False
	while not finished:
		nodes = []
		fixed_spots = product(range(COLORS), repeat=NB_FIXED_SPOTS)
		j = 0
		for fixed_spot in fixed_spots:
			node = Node(j, fixed_spot)
			node.prev_guesses = prev_guesses[j]
			node.prev_scores = prev_scores[j]
			nodes.append(node)
			j += 1
		
		for i in range(NB_NODES):
			nodes[i].start()

		for i in range(NB_NODES):
			nodes[i].join()
		
		for i in range(NB_NODES):
			guess = nodes[i].new_guess
			if guess == None: continue
			result = score(guess, secret)
			prev_guesses[i].append(guess)
			prev_scores[i].append(result)
			if guess == secret:
				finished = True
				solutionFound = guess
				break
	
	print("Found :", solutionFound)
	
def main():
	timeStart = time.time()
	masterNode()
	print("Time spend: ", time.time() - timeStart)
	


if __name__ == "__main__":
	try:
		main()
	except KeyboardInterrupt:   # Handle CTRL-C
		exitApp = True
		print("STOP")

"""
Node:
	prev_guests  = [ (guess), (guess2) ,...  ]
	prev_scoress  = [ (score), (score2) ,...  ]

	combis = product(range(colors), spots-NB_NODES) #diff thread (fix NBNODES first positions)
	for combi in combis:
		flag = True
		for i in range(len(prev_scoress)):
			if score(prev_guest[i], combi) == prev_scores[i]:
				flag = False
				break
		if flag:
			return combi #return au master

Master:
	prev_guest = []
	prev_scores = []
	nodes = []
	
	solution found= False
	make random init guess
	while not solution found:
		for node in nodes:
			node.set prev_scores
			node.set prev_guests 
			node.setFixedBegin(begin)
		
		for node in nodes:
			node.start()
		for node in nodes:
			node.join()

		evaluate nodes guesses
		if sol found :
			solution found = True
		
	print( "YAS QUEEN SLAY BITCH FDP")
"""