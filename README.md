# Order-Book
This order book system is designed to simulate and process financial orders in a dynamic and configurable manner. It incorporates essential trading mechanisms such as order generation, market sentiment adjustments, and real-time processing of buy and sell orders. Here’s an overview of its functionality:

## Order Types:

Limit Orders: Users specify a price at which they want to buy or sell shares.

Market Orders: Orders are executed immediately at the current market price.


## Dynamic User Configurations:

Number of Orders (trades): The user defines the total number of orders to be generated and processed.

Min/Max Shares (shares_limit): Users can set limits on the quantity of shares that can be bought or sold in a single order.

Market Sentiment (sentiment): Controls the bias toward buy (bullish) or sell (bearish) orders. A value closer to 1 indicates strong bullish sentiment, while values near 0 indicate bearish sentiment.

Price Spread (spread): Determines the price difference between the highest bid (buy) and the lowest ask (sell). This affects market liquidity and pricing volatility.

Initial Stock Price (current_price): Sets the starting price for the stock being traded.


## How to run

To run the order book simulation, simply copy the provided code and paste it into a Jupyter Notebook cell. Ensure you have the necessary dependencies, such as matplotlib, installed in your environment. Execute the cell to initialize the global parameters like current_price, spread, trades, and sentiment, and generate a specified number of orders. As the simulation progresses, the code will display the current price, the order book (including buy and sell orders), and the volume traded at each price point through a graph. This visualization helps in understanding the dynamics of the market in real-time.


### Note

The original implementation of the order book system was written in C++ as orderBook.cpp. Later, the code was converted into Python to improve visualization, making it easier to interact with and understand the system's behavior.
