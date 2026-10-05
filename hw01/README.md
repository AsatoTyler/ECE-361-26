# Homework 1

This library provides functions for bit manipulation and bit value representation. The functions allow for the extraction and setting of bit fields, 32-bit int to binary conversion, and two's compliment sign extension. 

# Built and test

To build obhect files, run:
'make'

To run tests, run:
'make test'

To remove generated build files, run:
'make clean'

# Input ranges and boundaries

**Valid Inputs:** Expects a 'width' value of 1-32, a 'pos' value of 0-31, and a 'pos + width' value no greater than 32.

**Out of bounds behavior:** If 'get_field' or 'set_field' recieve a value outside of their valid bounds, they abort and return 0 or the original 'word' respectively. If 'sign_extend' recives a width outside of its bounds returns 0.