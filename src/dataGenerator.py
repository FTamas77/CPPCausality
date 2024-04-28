import numpy as np

# Set a random seed for reproducibility
np.random.seed(0)

# Generate data for five variables
A = np.random.randint(1, 101, size=100)
B = A + np.random.randint(1, 101, size=100)
C = B + np.random.randint(1, 101, size=100)
D = np.random.randint(1, 101, size=100)
E = np.random.randint(1, 101, size=100)

# Combine the variables into a dataset
data = np.column_stack([A, B, C, D, E])

# Save the dataset to a CSV file
np.savetxt("data.csv", data, delimiter=",", fmt='%i')

# The output might look something like this:
# A --> B
# B --> C
# D
# E