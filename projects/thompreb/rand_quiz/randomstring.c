/*********************************************************************

In the interest of reducing runtime, I limited the possible characters
that the inputChar() and inputString() select from to just the
characters that the test function is concerned with, plus one
additional non-target character to test for that case. Since their
order is still randomized, limiting the options in this way should not
negatively impact the test's integrity.

I also decided not to treat the null terminating character '\0' as
one of the characters that inputString() can select when building its
string. The null terminating character will still be appended as usual
to the end of the string.

Aside from that, it was just a matter of taking advantage of the fact
that strings are represented as arrays to randomly select the index
of a character from the string of all possible characters, and either
return it or add it to the randomized string as required.

The testme() function will loop until certain chars have appeared
often enough for state to reach 9, and until the string is randomized
to "reset", at which point it will print the error and quit.

*********************************************************************/
