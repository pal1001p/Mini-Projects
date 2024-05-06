/***************************************************
* void print_item(int rows, int cols, int data[][cols])
* Prints the sales of an item asked for by the user
*
* Parameters:
* rows: the number of rows in data
* cols: the number of cols in the data
* dataArray: A 2D array of the sales data
* Returns:
* void
************************************************/
void print_item(int rows, int cols, int dataArray[][cols]);

/***************************************************
* void total_sales(int rows, int cols, int data[][cols])
* Determine the total sales of each item
* Prints the results to sales.txt
*
* Parameters:
* rows: the number of rows in data
* cols: the number of cols in the data
* dataArray: A 2D array of the sales data
* Returns:
* void
************************************************/
void total_sales(int rows, int cols, int dataArray[][cols]);

/***************************************************
* void top_overall(int rows, int cols, int data[][cols])
* Finds the total sales of each item
* Prints the most popular item across all months to the terminal
*
* Parameters:
* rows: the number of rows in data
* cols: the number of columns in data
* dataArray: A 2D array of the sales data
* Returns:
* void
************************************************/
void top_overall(int rows, int cols, int dataArray[][cols]);