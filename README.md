# forecast
A very simple script to forecast a given amount based on deterministic/stochastic rules in time

# Compile with :
```
g++ fc.cpp
```

# run:
```
./a.out example.dat
```

# syntax of example.dat (# are comments, they must not be present in the dat file.):
```
100                 # number 

quotidien           # marker for daily events
eat		-7            # list of daily events 

hebdo               # marker for weekly events
restaurant 	-30     # list of weekly events (loss are negative)
casino 		100       # (gains are positive)

mensuel
rent 		-600
salary 		3000
bills 		-200

ponctuel                #once a year events
taxes 		-1000 	150   # <name> <loss/gain> <numberofthedayintheyear>
bigtrip 	-1500 	80    #

random
randomevent1	-100 	0.0001 # <name> <loss/gain> <probability this event happens each day>
randomevent2 	-10 	0.001 
```
