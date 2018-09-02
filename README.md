# Wave_Market_Analysis
Applies aspects of the Elliot Wave Theory to analyze financial markets.

Currently takes input from a CSV file:
  - designed to smart search the CSV file to index the data points
 
Does a full spectrum search:
  - computes using the smallest available interval, then recomputes with an increment to the interaval, until the interval is too large for the dataset
  - this allows the specified wave theory to be analyzed both short term and long term. It will find every intance of the pattern.

Output is basic for now, working on algorithms atm

BETA:
  - Will search for a motive impulse wave pattern in any csv with open and close values, at any interval
  - Will provide feedback of a range of row #'s where the match can be found in the entered CSV
