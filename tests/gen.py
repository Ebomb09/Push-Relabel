import random
import subprocess

tests = 10
V = 100

def create_test(name):
	E = V*(V-1)

	file = open(f'tests/{name}', "w")
	file.truncate(0)

	file.write(str(V) + "\n")

	for u in range(0, V):
		for v in range(0, V):

			if(u != v):
				flow = random.randint(1, 1000)
				file.write(f'{u} {v} {flow}\n')

	file.close()

"""Create Tests"""
print('Creating Tests')

for x in range(0, tests):
	create_test(f'graph_{x}')
	print(f'[{x+1}/{tests}] Test Created')

"""Run Tests"""
print('Running Tests')

pr_out = open(f'data/RT_push_relabel_{V}.csv', "w")
pr_out.truncate(0)
ff_out = open(f'data/RT_ford_fulkerson_{V}.csv', "w")
ff_out.truncate(0)

for x in range(0, tests):
	pr_out.write(subprocess.check_output(['./bin/pr', f'tests/graph_{x}']).decode())
	ff_out.write(subprocess.check_output(['./bin/ff', f'tests/graph_{x}']).decode())
	print(f'[{x+1}/{tests}] Test Completed')

pr_out.close()
ff_out.close()