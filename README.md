# Wave_Market_Analysis
Applies aspects of the Elliot Wave Theory to analyze stock market data.

Currently takes input from an API generated CSV file:
  - designed to smart search the input CSV file and index the data variably according to column title
 
Does a full spectrum search:
  - computes the entered stock ticker's data using the smallest available interval, then recomputes with an increment to the interaval, until the interval is too large for the dataset
  - data is gathered directly from an API call upon entry in the live version
  - this allows the specified wave theory to be analyzed both short term and long term. It will find every intance of the pattern, from intervals of one minute to months.

Output is basic for now, working on algorithms atm

# To Use:
1. Download the Wave Theory Market Analysis folder
2. Open terminal and cd to your downloads folder, then cd to the downloaded folder
3. Compile the .c for yourself, or run the included ./live_tool or ./historical_tool
  - The ./live_tool executes with no input, gathering data from an api upon call by the program
  - The ./historical_tool takes a .csv file as argv[1]input during execution "./historical_tool folder/file.csv"
 
# How to Interpret Results
  - The program will output the interval and the price/date/time data for series of data points which match those found by the elliot wave theory
  - If you see "MOTIVE IMPULSE WAVE DETECTED", the stock, at some time and some interval, underwent conditions matching the coded pattern of the elliot wave theory.
  - It will look like this. I ran this on June 24, 2020 for AAPL:
        
        MOTIVE IMPULSE WAVE DETECTED
        1
        2020-06-18 14:30:00
        350.31
        2020-06-18 13:30:00
        351.64
        2020-06-18 12:30:00
        350.7834
        2020-06-18 11:30:00
        352.13
        2020-06-18 10:30:00
        351.65
        2020-06-18 09:30:00
        352.65
        ----------
        Downloading next update...
        
   - This means at an interval of 1 hour, AAPL's stock price matched the requirements of the elliot wave theory.
   - The Elliot Wave theory cites predicitve rises and falls, and so if found enough, gain can be made with relative risk
 
 # Next Steps:
Implement graphing functions and algorithms to plot this data
Create a predictive AI to email me the graphs of stocks which my program is confident will continue a certain trend
