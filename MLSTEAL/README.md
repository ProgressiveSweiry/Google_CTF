<div align="center">

<img src="../img/misc.png" width="64" height="64" /> 

# [MLSTEAL](https://capturetheflag.withgoogle.com/challenges/misc-ml-steal)

### "What's Alice's password?"

<br>

</div>

# Identify The File

* open file in hex editor
* first four bytes 50 4B 03 04 are the signature of ZIP file
* change file name to ***.zip
* working on windows, install WSL to inspect file 
* decompressed ml-steal.tar.xz with linux
* search for tar file signature 
  
  75 73 74 61 72 00 30 30 - 2 found

  FD 37 7A 58 5A 00 - this one is the start of the file tar.xz LZMA2 compression

# looking for the script

* ctf folder contains script, .so file and weights.

* inside the folder ml-steal/ctf there's a python script, ml weights and a library (.SO file)

* query.py trying to import :

    `# Import our fancy neural network
     from mlsteal import sample`

* check file mlsteal.cpython-38-x86_64-linux-gnu.so change its name to mlsteal.so to be able to import it 
* linux is needed to run the script (I'm using WSL)
* explore the library mlsteal:

<code>
  >python3
   >>> import mlsteal
   >>> help(mlsteal)

   Help on module mlsteal:

  NAME
    mlsteal

  FUNCTIONS
    sample(...) method of _dynfunc._Closure instance

  FILE
    ctf/mlsteal.so

  (END)

</code>

# Generating Text With mlsteal

*  run the script :

`Please pass one argument containing a string to prompt the language model`

<br>


* the script generate text based on the argument when calling the function

<code>
->./query.py T

->b'Territory.\n\nTerritory.\n\nGuo has come from the India army at Paris, near the Rhynie continental, inclu'
</code>

<br>

* created helper function to find string "CTF" in generated text


<code>

    def checkforkey(weights, counter):
        string = ''.join(random.choice(letters) for i in range(1))
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
    
</code>
<br>

# Alice's Password

after brute force generating text with this model, this string was printed a few times:

  `"Alice Bobson's password is CTF{"`


* change the random model input  
<br>
   `string = "Alice Bobson's password is CTF{" +''.join(random.choice(letters) for i in range(1)) `

* after generating some text we get some repeating strings like 
<code>
p3m1r1z4t10n-1S-4LL
z3m0r1z4t10n-1S-4LL
y0n-1S-4LL
k3m0r1z4t10n-1ST-
m3m0r1z4t10n-1S-6LL
m3m0r1z4t10n-1S-4LL-
</code>
<br>

* change the random model input to check to iterate through the ascii table (31 is the offset to get char compatible with flag format) 
<br>

<code>
string = "Alice Bobson's password is CTF{m3m0r1z4t10n-1S-4LL-" + chr(31 + counter)

</code>
<br>

* we get this:
<br>

`String For Result: Alice Bobson's password is CTF{m3m0r1z4t10n-1S-4LL-y
b"Alice Bobson's password is CTF{m3m0r1z4t10n-1S-4LL-y0u-N334\n\n\n\nCrain Forest Laurel\n\n\n\nThe Crain Forest Forest Laurel, the Crain Forest Forest Labduction"`

<br>

* change string:

`string = "Alice Bobson's password is CTF{m3m0r1z4t10n-1S-4LL-y0u-N33"`

* this is probably the sign we got the flag (N33D2 vs/N33D})

`String For Result: Alice Bobson's password is CTF{m3m0r1z4t10n-1S-4LL-y0u-N33D2
b"Alice Bobson's password is CTF{m3m0r1z4t10n-1S-4LL-y0u-N33D2 vs/N33D}\n\n\n\n\n\n\n\nScottish English Scots\n\n\n\n\n\n\n\nOrthodoxies\n\n\n\n\n\n\n\nMirror azures\n\n\n\nOrthodoxies are a"`

* trying this 

`string = "Alice Bobson's password is CTF{m3m0r1z4t10n-1S-4LL-y0u-N33D}" `

<div align=center>


we get at least two '\n' char with the rest of the text being readable, it means we got the flag


 THE FLAG: 
  ### **"CTF{m3m0r1z4t10n-1S-4LL-y0u-N33D}"**

</div>