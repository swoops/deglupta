# deglupta
Recursive password mutator to REALLY blow up those password files.
# why not just use --PROGRAM--
I have not been able to find any password mutators that recursively mutate, such
as taking "foo" to "f0o", "f00" and "fo0".  Likely because recursion will 
exponentially increase the output and now days the name of the cracking game is 
to limit your key space.  However, few things swoopses my goat more then finding 
out you missed out on cracking a WPA PSK handshake because you only tried 
"C0mp@ny 1nc", and not "Com@ny Inc", "C0mpany Inc" or "C0mpany inc".  So I 
consider it a worth while endeavor to REALLY mutate a very small list of strong 
possibilities and then slightly mutate a much larger typical password list 
(rock_you, 18-in-1, ect).
# why C?
I did write this in python at first, however oclHashcat would burn through the 
output twice as fast as it was created.  So I could either add threading or 
write it in a more efferent language.  Adding threading would eat more CPU when 
I wanted to pipe the output to John or something.  So instead I wrote it in C.  
Also C is fun!
# Adding your own mutations
Compiling the code is really easy:
```
$ time gcc main.c; echo $?

real	0m0.058s
user	0m0.044s
sys	0m0.008s
0

```
So to add your own function just modify the source and recompile.  If you
want to add something real quick, just modify the "identity" function found
in the main.c file.  Currently it does nothing but pass the password along
to the next function, but it is accessible via the command line.

If you want your new function to be more permanently, then model it after one 
that exists already.  In the main.h file add a function prototype, add an entry
and modify the case statement and the string passed to printf int the
get_da_func function. Then add your new function to the bottom of the main.c
file.  It should be a void function accepting PARAMS, like all the other
functions.  See the comments in the identity function for more information.  
# YOUR CODE IS BAD AND YOU SHOULD FEEL BAD
If you find something wrong in my code I would love to hear about it.  If it
is neat or cool I may find some way to reward you with a t-shirt, something
I have on my desk, or some unpublished piece of code I have.
# TODO
* Replace PARAMS with a struct
* Add more functions.
* Make it more modular.
* set up some good default function lists
