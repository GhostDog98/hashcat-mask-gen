kk# hashcat mask gen
This program is designed to create hashcat friendly formatted masks for a given AD password policy.
This can produce approximately 50 million masks per second on an AMD Ryzen 9 3600X assuming writing to memory.


To calculate how many masks per second your setup can make, use the following:
`make && multitime -n 10 sudo nice -n -20 ./password_masks 1 13 1 1 1 1 > /dev/null`

Then, divide the time you got by `80010804`. I use wolfram alpha to do this so I just enter `80010804 / (2.104 sec)`

This development began in 2018, and has eventually evolved from a python script, into bash, into C.
I am still looking for ways to make this better too, so feel free to open a commit or issue if desired.


# Sample usage
`./password_masks 1 12 1 2 3 4` generates passwords between 1 and 12 length, with a minimum of 1 lowercase, 2 uppercase, 3 digit, and 4 special characters.

This means the real minimum length it will generate is infact 1 + 2 + 3 + 4, or 10, but it can figure this out itself. 

