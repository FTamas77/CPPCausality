import sys, os
sys.path.append(os.path.join(sys.path[0],'out','build','x64-Debug'))

from engine import fast_tanh2

print(fast_tanh2(10))
print(fast_tanh2(10))
