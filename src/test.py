import sys, os
parent = os.path.join(sys.path[0],'..')
sys.path.append(os.path.join(parent,'build','Debug', 'bin'))

from engine_api import add_numbers

print(add_numbers(10, 10))
