#!/usr/bin/python3

import sys
import gzip
import pickle
import numpy as np

import random
import string

from numba import njit, jit, cuda
import numpy as np

# Import our fancy neural network
from mlsteal import sample

letters = string.ascii_lowercase

def checkforkey(weights, counter):
    string = "Alice Bobson's password is CTF{m3m0r1z4t10n-1S-4LL-y0u-N33D}" 
    prompt = bytes(string,'ascii')
    x = np.array(list(prompt), dtype=np.uint64)

    genereted = b"".join(np.array(sample(x, weights),dtype=np.uint8))
    counter += 1


    if(genereted.find(b"CTF") > 0):
        print("String For Result: " + string)
        print(genereted)
        print("------------------------------------------")
        checkforkey(weights, counter)
    else:
        print("TESTING... " + str(counter) + " String: " + string)
        checkforkey(weights, counter)
    

if __name__ == "__main__":
    # Load the model weights
    weights = np.frombuffer(gzip.open("weights.gz","rb").read(), dtype=np.float32)


    checkforkey(weights, 0)

    # Get model prompt from first argument
    if len(sys.argv) == 1:
        print("Please pass one argument containing a string to prompt the language model")
        exit(1)
    prompt = bytes(sys.argv[1],'ascii')
    x = np.array(list(prompt), dtype=np.uint64)

    # Run neural network on the prompt
    y = sample(x, weights)

    # Print to stdout
    print(b"".join(np.array(sample(x, weights),dtype=np.uint8)))
