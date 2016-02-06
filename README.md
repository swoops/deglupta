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
# YOUR CODE IS BAD AND YOU SHOULD FEEL BAD
If you find something wrong in my code I would love to hear about it.  If it
is neat or cool I may find some way to reward you with a t-shirt, something
I have on my desk, or some unpublished piece of code I have.
# TODO
* Clean up help menu some
* Add more functions.
* Make it more modular.
* set up some good default function lists
