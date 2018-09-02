# Wave_Market_Analysis
Applies aspects of the Elliot Wave Theory to analyze financial markets.

Currently takes input from a CSV file:
  - designed to smart search the CSV file to index the data points
 
Does a full spectrum search:
  - computes using the smallest available interval, then recomputes with an increment to the interaval, until the interval is too large for the dataset
  - this allows the specified wave theory to be analyzed both short term and long term. It will find every intance of the pattern.

Output is basic for now, working on algorithms atm
