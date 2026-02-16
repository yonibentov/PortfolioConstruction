/* Optimal trading of geometric mean-reversion */

#include "basePath.h"
#include "common.h"
#include "array.h"
#include "matrix.h"
#include "volume.h"
#include "csv.h"
#include "setup.h"
#include "dynamicProgramming.h"

void game_setArguments(Game* game, Int numberOfArguments, char** arguments);
void game_print(Game* game);
void game_write(Game* game, String* filepath);
void portfolio_arrayFloat_createGrids(Game* game);

int main(int numberOfArguments, char** arguments) {
  printf("base path: %s\n", basePathLiteral);
  //  return 0;
  
  /*******************/
  /* Set up the game */
  /*******************/
  
  printf("Setting up the game...\n");
  Game game = {
    .numberOfGames = 1,

    .temporalIncrement = 0.01,  // dt
    .totalTime = 1,  // T

    .logPriceIncrement = 0.10,  // dx
    .maximumLogPrice = 10,  // x_cutoff

    .logWealthIncrement = 0.10,  // dy
    .maximumLogWealth = 10,  // y_cutoff

    .volatility = 1.00, // sigma
    .asymptoticMean = 0.00,  // m
    .meanReversionSpeed = 0.01,  // theta

    .interestRate = 0.01,  // r
  };

  /* Overwrite default values */
  if (numberOfArguments > 1) {
    game_setArguments(&game, numberOfArguments, arguments);
  }

  game.numberOfRoundsPerGame = (Int)floor(game.totalTime/game.temporalIncrement);
  portfolio_arrayFloat_createGrids(&game);
  game_print(&game);
  printf("Finished setting up the game.\n");


  /*********************/
  /* Set up the player */
  /*********************/

  printf("Setting up the player...\n");

  UtilityFunction utilityFunction;
  Player player = {
    .fractionInCash = arrayFloat_create(game.numberOfRoundsPerGame+1),  // 1-w
    .fractionInStock = arrayFloat_create(game.numberOfRoundsPerGame+1),  // w
    .logWealth = arrayFloat_create(game.numberOfRoundsPerGame+1),  // y(t)
    .trades = arrayFloat_create(game.numberOfRoundsPerGame),  // dN(t)
    .utilityFunction = &utilityFunction
  };
  
  printf("Finished setting up the player.\n");

  
  /******************************************/
  /* Set up the base directory for file I/O */
  /******************************************/

  String* basePath = string_create();
  //  string_overwriteWithCharacters(basePath,
				 //				 #if defined __linux__
				 //    "/home/yoni/Dropbox/Finance/Research 2025/Portfolio Construction/files/examples/"
				 //				 #elif defined __APPLE__
				 //    "/Users/yoni/Library/CloudStorage/Dropbox/Finance/Research 2025/Portfolio Construction/files/examples/"
				 //				 #endif
				 //				 );
  string_overwriteWithCharacters(basePath, basePathLiteral);
  
  printf("Base path for file I/O:\n");
  string_print(basePath);
  //  return 0;
  
  /* Create export information, which will be completed later
     for each utility function */
  String* exportPath = string_create();
  Data exportData = {
    .file = NULL,
    .numberOfRows = game.numberOfGames,
    .numberOfColumns = game.numberOfRoundsPerGame + 1
  };

  
  /****************************************/
  /* Write the game parameters to storage */
  /****************************************/

  String* simulationPath = string_create();
  string_overwriteWithString(simulationPath,  // destination
			     basePath);  // source
  string_appendCharacters(simulationPath,
			  "gameParameters.txt");
  game_write(&game, simulationPath);



  /******************************************/
  /* Set up the dynamic-programming problem */
  /******************************************/

  printf("Setting up the dynamic-programming problem...\n");

  MatrixFloat* returns = matrixFloat_create(game.sizeOfLogPriceGrid,
					    game.sizeOfLogWealthGrid);
  VolumeFloat* policies = volumeFloat_create(game.sizeOfLogPriceGrid,
					     game.sizeOfLogWealthGrid,
					     game.numberOfRoundsPerGame);

  printf("Finished setting up the dynamic-programming problem.\n");

  
  /*****************************************/
  /* Set up the reading of a price history */
  /*****************************************/

  ArrayFloat* priceHistory = arrayFloat_create(game.numberOfRoundsPerGame + 1);
  String* importPath = string_create();
  Data importData = {
    .file = NULL,
    .numberOfRows = game.numberOfGames,
    .numberOfColumns = game.numberOfRoundsPerGame + 1
  };

  
  /*****************************/
  /* Loop over simulation runs */
  /*****************************/
  #define NUMBER_OF_RUNS 5
  char* runs[NUMBER_OF_RUNS] = {"1","2","3","4","5"};
  for (Int R = 0; R < NUMBER_OF_RUNS; R += 1) {
    printf("\n*****\n");
    printf("Run %lld\n", R+1);
    printf("*****\n\n");

    
    /*************************************/
    /* Read a price history from storage */
    /*************************************/

    printf("About to read in a price history...\n");
  
    string_overwriteWithString(importPath,  // destination
			       basePath);  // source
    string_appendCharacters(importPath, runs[R]);
    string_appendCharacters(importPath,
			    "/priceHistory.csv");
    printf("Import path:\n");
    string_print(importPath);
    
    importData.file = fopenFromString(importPath, "r");
    csv_arrayFloatFromData(priceHistory,
			   &importData);
    fclose(importData.file);
    
    printf("Finished reading in a price history.\n\n");


    /**********************************************************/
    /* Calculate the optimal policy for each utility function */
    /**********************************************************/

    #define NUMBER_OF_UTILITY_FUNCTIONS 2
    Int availableUtilityFunctions[NUMBER_OF_UTILITY_FUNCTIONS] = {
      LOG_UTILITY, TARGET_UTILITY
    };

    for (Int u = 0; u < NUMBER_OF_UTILITY_FUNCTIONS; u += 1) {
      setUtilityFunction(player.utilityFunction,
			 availableUtilityFunctions[u]);

      printf("About to calculate the optimal policy under utility function %s...\n",
	     player.utilityFunction->name);

      portfolio_determineOptimalPolicy(policies, returns,
				       &game, &player);

      printf("Finished calculating the optimal policy.\n\n");


      /***********************************************/
      /* Play a game according to the optimal policy */
      /***********************************************/

      printf("About to backtest the policy...\n");

      portfolio_backtest(policies, &game, priceHistory, &player);

      printf("Finished backtesting the policy.\n\n");


      /*********************************************/
      /* Write the player's performance to storage */
      /*********************************************/

      printf("About to write the results to storage...\n");

      // Write the performance results
      string_overwriteWithString(exportPath,  // destination
				 basePath);  // source
      string_appendCharacters(exportPath, runs[R]);
      string_appendCharacters(exportPath,
			      "/performance/performance_");
      string_appendCharacters(exportPath,
			      player.utilityFunction->name);
      string_appendCharacters(exportPath,
			      ".csv");
      string_print(exportPath);

      exportData.file = fopenFromString(exportPath, "w");
      csv_dataFromArrayFloat(&exportData,
			     player.logWealth);
      fclose(exportData.file);
      
      // Write the policy
      string_overwriteWithString(exportPath,
				 basePath);
      string_appendCharacters(exportPath, runs[R]);
      string_appendCharacters(exportPath,
			      "/performance/policy_");
      string_appendCharacters(exportPath,
			      player.utilityFunction->name);
      string_appendCharacters(exportPath,
			      ".csv");
      string_print(exportPath);

      exportData.file = fopenFromString(exportPath, "w");
      csv_dataFromArrayFloat(&exportData,
			     player.fractionInStock);
      fclose(exportData.file);
      
      // Write the trades
      string_overwriteWithString(exportPath,
				 basePath);
      string_appendCharacters(exportPath, runs[R]);
      string_appendCharacters(exportPath,
			      "/performance/trades_");
      string_appendCharacters(exportPath,
			      player.utilityFunction->name);
      string_appendCharacters(exportPath,
			      ".csv");
      string_print(exportPath);

      exportData.file = fopenFromString(exportPath, "w");
      csv_dataFromArrayFloat(&exportData,
			     player.trades);
      fclose(exportData.file);
      
      printf("Finished writing the results to storage.\n\n");

    }  // End loop over utility functions

  
    /*************************************************/
    /* For comparison, test a few arbitrary policies */
    /*************************************************/

    /* Buy-and-hold: 0%, 50%, 100% */

    /* All cash (0% stock) */
    printf("About to test an all-cash policy...\n");
  
    volumeFloat_setAll(policies, 0);
    portfolio_backtest(policies, &game, priceHistory, &player);
  
    printf("Finished testing an all-cash policy.\n");
  
    // Write the performance results
    printf("About to write results to storage...\n");
  
    string_overwriteWithString(exportPath,  // destination
			       basePath);  // source
    string_appendCharacters(exportPath, runs[R]);
    string_appendCharacters(exportPath,
			    "/performance/performance_allCash.csv");
    string_print(exportPath);

    exportData.file = fopenFromString(exportPath, "w");
    csv_dataFromArrayFloat(&exportData,
			   player.logWealth);
    fclose(exportData.file);
      
    printf("Finished writing results to storage.\n\n");
  
    /* All stock */
    printf("About to test an all-stock policy...\n");

    volumeFloat_setAll(policies, 1);
    portfolio_backtest(policies, &game, priceHistory, &player);
    
    printf("Finished testing an all-stock policy.\n");
  
    // Write the performance results
    printf("About to write results to storage...\n");
  
    string_overwriteWithString(exportPath,  // destination
			       basePath);  // source
    string_appendCharacters(exportPath, runs[R]);
    string_appendCharacters(exportPath,
			    "/performance/performance_allStock.csv");
    string_print(exportPath);

    exportData.file = fopenFromString(exportPath, "w");
    csv_dataFromArrayFloat(&exportData,
			   player.logWealth);
    fclose(exportData.file);
    
    printf("Finished writing results to storage.\n\n");
  
    /* Buy and hold 50% requires some work to program;
       the simpler thing to program is to maintain a
       constant position of 50%. Note that maintaining a
       constant position of w = 50% is a dynamic policy,
       requiring continuous rebalancing; whereas, the way
       I've organized things, spending 50% of your cash
       on stock on the first update, then doing nothing
       will mean that w = NS/W changes with time. In that
       case, it is *N* that is constant; S and W will
       change with time, leading to w changing with time. */
  
    /* Maintain a constant position of 50% */
    printf("About to test a dynamic 50/50 policy...\n");
  
    volumeFloat_setAll(policies, 0.5);
    portfolio_backtest(policies, &game, priceHistory, &player);

    printf("Finished testing a dynamic 50/50 policy.\n");
  
    // Write the performance results
    printf("About to write results to storage...\n");
  
    string_overwriteWithString(exportPath,  // destination
			       basePath);  // source
    string_appendCharacters(exportPath, runs[R]);
    string_appendCharacters(exportPath,
			    "/performance/performance_half.csv");
    string_print(exportPath);

    exportData.file = fopenFromString(exportPath, "w");
    csv_dataFromArrayFloat(&exportData,
			   player.logWealth);
    fclose(exportData.file);
    
    printf("Finished writing results to storage.\n\n");

  }  // End loop over runs

  
  /******************************************/
  /* Destroy everything taken from the heap */
  /******************************************/

  printf("About to clean up...\n");
  
  // Game
  arrayFloat_destroy(game.logPriceGrid);
  arrayFloat_destroy(game.logWealthGrid);

  // Player
  arrayFloat_destroy(player.fractionInCash);
  arrayFloat_destroy(player.fractionInStock);
  arrayFloat_destroy(player.logWealth);
  arrayFloat_destroy(player.trades);
  
  // Dynamic programming
  matrixFloat_destroy(returns);
  volumeFloat_destroy(policies);
  
  // Backtesting
  arrayFloat_destroy(priceHistory);
  
  // File I/O
  string_destroy(basePath);
  string_destroy(simulationPath);

  string_destroy(importPath);
  string_destroy(exportPath);
  

  DONE;
}


#include <assert.h>

void game_setArguments(Game* game, Int numberOfArguments, char** arguments) {
  assert(game);
  assert(numberOfArguments > 1);
  assert(numberOfArguments < 6);  /* 1: program name (C convention),
				     2: volatility,
				     3: asymptotic mean,
				     4: mean-reversion speed,
				     5: interest rate */
  game->volatility = atof(arguments[1]);
  if (numberOfArguments > 2) {
    game->asymptoticMean = atof(arguments[2]);
  }
  if (numberOfArguments > 3) {
    game->meanReversionSpeed = atof(arguments[3]);
  }
  if (numberOfArguments > 4) {
    game->interestRate = atof(arguments[4]);
  }
}

void game_print(Game* game) {
  printf("Game:\n");
  printf("  Number of games: %lld\n", game->numberOfGames);
  printf("  Temporal increment: %0.4f\n", game->temporalIncrement);
  printf("  Total time: %0.4f\n", game->totalTime);
  printf("  Number of rounds per game: %lld\n", game->numberOfRoundsPerGame);
  printf("  Log-price increment: %0.4f\n", game->logPriceIncrement);
  printf("  Maximum log-price: %0.4f\n", game->maximumLogPrice);
  printf("  Size of log-price grid: %lld\n", game->sizeOfLogPriceGrid);
  printf("  Log-wealth increment: %0.4f\n", game->logWealthIncrement);
  printf("  Maximum log-wealth: %0.4f\n", game->maximumLogWealth);
  printf("  Size of log-wealth grid: %lld\n", game->sizeOfLogWealthGrid);
  printf("  Volatility: %0.4f\n", game->volatility);
  printf("  Asymptotic mean: %0.4f\n", game->asymptoticMean);
  printf("  Mean-reversion speed: %0.4f\n", game->meanReversionSpeed);
  printf("  Interest rate: %0.4f\n", game->interestRate);
}

void game_write(Game* game, String* filepath) {
  FILE* file = fopenFromString(filepath, "w");

  fprintf(file, "***************\n");
  fprintf(file, "Game parameters\n");
  fprintf(file, "***************\n\n");

  fprintf(file, "Number of games: %lld\n", game->numberOfGames);
  fprintf(file, "Temporal increment: %0.4f\n", game->temporalIncrement);
  fprintf(file, "Total time: %0.4f\n", game->totalTime);
  fprintf(file, "Number of rounds per game: %lld\n", game->numberOfRoundsPerGame);
  fprintf(file, "Log-price increment: %0.4f\n", game->logPriceIncrement);
  fprintf(file, "Maximum log-price: %0.4f\n", game->maximumLogPrice);
  fprintf(file, "Size of log-price grid: %lld\n", game->sizeOfLogPriceGrid);
  fprintf(file, "Log-wealth increment: %0.4f\n", game->logWealthIncrement);
  fprintf(file, "Maximum log-wealth: %0.4f\n", game->maximumLogWealth);
  fprintf(file, "Size of log-wealth grid: %lld\n", game->sizeOfLogWealthGrid);
  fprintf(file, "Volatility: %0.4f\n", game->volatility);
  fprintf(file, "Asymptotic mean: %0.4f\n", game->asymptoticMean);
  fprintf(file, "Mean-reversion speed: %0.4f\n", game->meanReversionSpeed);
  fprintf(file, "Interest rate: %0.4f\n", game->interestRate);
  
  fclose(file);
}

void  portfolio_arrayFloat_createGrids(Game* game) {
  assert(game);

  /* Assert valid dt, T, x_max, and y_max */
  assert(game->temporalIncrement);
  assert(game->temporalIncrement > 0);
  assert(game->totalTime);
  assert(game->totalTime > game->temporalIncrement);
  assert(game->totalTime < LARGEST_FLOAT);

  assert(game->logPriceIncrement);
  assert(game->logPriceIncrement > 0);
  assert(game->maximumLogPrice);
  assert(game->maximumLogPrice > 2.0*game->logPriceIncrement);
  assert(game->maximumLogPrice < LARGEST_FLOAT);

  assert(game->logWealthIncrement);
  assert(game->logWealthIncrement > 0);
  assert(game->maximumLogWealth);
  assert(game->maximumLogWealth > 2.0*game->logWealthIncrement);
  assert(game->maximumLogWealth < LARGEST_FLOAT);

  /* Create the log-price (x) grid */
  game->sizeOfLogPriceGrid = 1+(Int)floor(2*game->maximumLogPrice/game->logPriceIncrement);
  game->logPriceGrid = arrayFloat_create(game->sizeOfLogPriceGrid);
  for (Int i = 0; i < arrayFloat_getSize(game->logPriceGrid); i += 1) {
    arrayFloat_setAt(game->logPriceGrid, i,
		     -game->maximumLogPrice + (game->logPriceIncrement)*((Float)i));
  }

  /* Create the log-wealth (y) grid */
  game->sizeOfLogWealthGrid = 1+(Int)floor(2*game->maximumLogWealth/game->logWealthIncrement);
  game->logWealthGrid = arrayFloat_create(game->sizeOfLogWealthGrid);
  for (Int i = 0; i < arrayFloat_getSize(game->logWealthGrid); i += 1) {
    arrayFloat_setAt(game->logWealthGrid, i,
		     -game->maximumLogWealth + (game->logWealthIncrement)*((Float)i));
  }
}
