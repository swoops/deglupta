# deglupta
Recursive password mutator to REALLY blow up those password files.
# NOTE
This is not done, and is not intended to be used just yet (though it could be). 
I only published it so I could get input and help from some friends more easily.
Additionally, version control is only helpful when you are changing the code. :)
# why not just use --PROGRAM--
I have not been able to find any password mutators that recursively mutate, such
as taking "foo" to "f0o", "f00" and "fo0".  Likely because recursion will 
exponentially increase the output and now days the name of the cracking game is 
to limit your key space.  However, it really stinks when you find out you missed 
out on cracking a WPA PSK handshake because you only tried "C0mp@ny 1nc", and 
not "Com@ny Inc", "C0mpany Inc" or "C0mpany inc".  So I consider it a worth 
while endeavor to REALLY mutate a very small list of strong possibilities and 
then slightly mutate a much larger typical password list (rock_you, 18-in-1, 
ect).
# why C?
I did write this in python at first, however oclHashcat would burn through the 
output twice as fast as it was created.  So I could either add threading or 
write it in a more efferent language.  Adding threading would eat more CPU when 
I wanted to pipe the output to John or something.  So instead I wrote it in C.  
Also C is fun!
# TODO
* Allow users to submit a list of functions to run through.
* Add more functions.
* Make it more modular.
* KILL on file size limit
* Resume from the last word

