# Nand 2 Tetris

### Uploading Files
https://www.wikihow.com/Make-a-Zip-File


https://github.com/Gunasekare/nand2tetris/blob/master/projectfiles/00/file.txt
This github might be useful to download files directly (which make them work for uploading the assignments?)
idk, something weird with the files we download directly from their website, so this might help.


## Week 1
AND OR NOT
AND will work as follows:

0 AND 0 = 0
0 AND 1 = 0
1 AND 1 = 1

OR will work as follows:

0 OR 0 = 0
0 OR 1 = 1
1 OR 1 = 1

NOT (unary - accepts only 1 input) works as follows:
NOT 1 = 0
NOT 0 = 1

The cool thing about using only bools values, is that it allows us to have a table of truth (aka finite number of options and solution) as shown above.

We can also use a function with a few variables and use the same logic:
f(x, y, z) = (NOT(x) AND (y OR z))
For this function as well, we can list all the different possibilties that will occur if we'll assign 0 or 1 to x,y,z.

##### Comutative Laws:
These are operators that will always be equal if swapping the arrangment:
(x AND y) = (y AND x)
(x OR y) = (y OR x)

The main idea is that we can simplify our expression into simpler expression using different laws.
There are different laws that proof we can simplify.

##### NAND
NAND is essentially NOT AND
Which means
(x AND y) = NOT(x NAND y)


##### Interfaces & Implementations
The user of the product is interested in the abstract instruction of how the product works.
In the other hand, the auther of the product is likely more interested in how the product is implemented!
The abstract definition will be easier to grasp and less technical (hopefully) since we want to give the user less friction and good experience.

########### CONTINUE ###############
https://www.coursera.org/learn/build-a-computer/lecture/jmAls/unit-1-5-hardware-simulation
(14min)
