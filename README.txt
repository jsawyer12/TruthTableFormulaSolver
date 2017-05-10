README

ttable.c is a program that accepts 2 arguments, an integer up to 26 representing
the total number of variables, and an equation in single quotes, and creates and prints a truth 
table solving the equation for the given number of variables. 

To run, use the "make ttable" command to compile, and use "./ttable <number> '<equation>'"
command to run it. An example run would be:

./ttable 5 'ab-#cc-&de='

the program will automatically remove any invalid characters, values, and variables from the equation
so that the solver will be able to function smoothly no matter what is input into the equation.
to perform the computations, the program recognizes Reverse Polish Notation, and uses the characters
'|' for or, '&' for and, '#' for exclusive or, '>' for implication, and '='
for equivalence. You may also use '-' to negate a preceding arg. enjoy!