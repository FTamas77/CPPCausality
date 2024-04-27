import sys, os

parent = os.path.join(sys.path[0],'..')
if sys.platform == 'linux':      
    sys.path.append(os.path.join(parent,'build','Debug', 'bin'))
elif sys.platform == 'win32':
    sys.path.append(os.path.join(parent,'build','Debug', 'bin'))
else:
    raise "Invalid platform"


from engine_api import add_numbers

print(add_numbers(10, 10))
