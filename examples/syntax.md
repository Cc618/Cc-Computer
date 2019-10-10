# Notes
This file serves to show some examples of the Assembler.
The assembler syntax is inspired by Python, thus the syntax
highlighter used for the examples in this mardown is Python.
Pay attention, not all instructions are compatible with
Python. 

# Instructions :
```python
# Call the debugger
trap

# Exit
exit
```

# Operations :
```python
x = 2
y = x
y += x
x++
```

# Go statements and Functions :
```python
# Call func_name
func_name()

@ func_name
	@ label

	# Simple jump
	label(@)

# Return to the parent function
leave
```

# Control Flow :
```python
@ loop

# Compares x and y
x ? y

# Go to loop if x lesser than y
loop(<=)
```

# Stack :
```python
# Push x on the stack
> x

# Pop to y
< y
```

# IO :
```python
# Print as int x
$ x
# Print a new line
$
# Print x as character
$c x
```
