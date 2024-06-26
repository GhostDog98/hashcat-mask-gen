# hashcat mask gen
This program is designed to create hashcat friendly formatted masks for a given AD password policy.
This can produce approximately 5 million masks per second on a 2020 Intel i5 macbook with 8gb of RAM.
I will add more benchmarks shortly and fix other bugs.

# Sample usage
`./password_masks 1 12 1 2 3 4` generates passwords between 1 and 12 length, with a minimum of 1 lowercase, 2 uppercase, 3 digit, and 4 special characters.
This means the real minimum length it will generate is infact 1 + 2 + 3 + 4, or 10, but it can figure this out itself. 
