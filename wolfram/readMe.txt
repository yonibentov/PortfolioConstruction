This directory contains the portion of the supplemental material written as Wolfram notebooks. 
(Wolfram version 14.1.0 on macOS 26.1.)

Notebook 1 produces the plots in Sec. 5.4, 
Notebook 2 processes the output of the C program called coin.out to produce the plots in Sec. 5.10.
Notebook 3 produces the plots in Sec. 7.9 and the inferences in Sec. 7.10. 
Notebook 4 produces the plots in Secs. 9.5 and 9.6.

Notebooks 5 and 6 are to be used with the C program called portfolio.out to produce the results of Sec. 11:
  Step 1: Run Notebook 5 to simulate price histories and write them to storage.
  Step 2: Run portfolio.out to produce a table of optimal policies for any history, and then compute and write to storage the performance of those policies on the particular histories produced by Step 1.
  Step 3: Run Notebook 6 to read in the performance results from Step 2 and write plots to storage.

This directory contains a subdirectory called "output." For all notebooks beside 5, there is nothing more that needs to be said. But when running Notebook 5, I did not want to accidentally overwrite the simulated price histories that were used in the most recent version of the paper.

Because of that, I require manually copying and pasting the files in the "priceHistory" subdirectory of "output" into the appropriate subdirectories of the top-level "files" directory.
The appropriate subdirectories of "files" are as follows.
  -- The file "simulationParameters.txt" should be copied into "files/examples";
  -- The file "priceHistory1.csv" should be copied into "files/examples/1";
  -- Similarly for "priceHistory2.csv", "priceHistory3.csv", etc.
