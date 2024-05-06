#include <stdio.h>
#include <stdlib.h>
#include "register.h"


void print_item(int rows, int cols, int dataArray[][cols]){
    //initializes the index of user's chosen item
    int userItem = -1;
    //prompts for and updates user's chosen item
    printf("Which item?\n");
    scanf("%d", &userItem);
    //hard-codes row of months names first
    printf("Jan\t""Feb\t""Mar\t""Apr\t""May\t""Jun\t"
           "Jul\t""Aug\t""Sep\t""Oct\t""Nov\t""Dec\n");
    //for each column from January to December, prints the value
    //from the array's given row (based on userItem) and column
    for(int i = 0; i < cols; i++){
        //handles formatting case for extra \t on last item
        if(i == 11){
            printf("%d", dataArray[userItem][i]);
        } else {
            printf("%d\t", dataArray[userItem][i]);
        }
    }
    printf("\n");
}


void total_sales(int rows, int cols, int dataArray[][cols]){
    //opens sales.txt to write in
    FILE* f0 = fopen("sales.txt", "w");
    //prints the header info
    fprintf(f0, "Item\tSales\n");
    //sums up the elements in each column, and iterates for each of the 50 rows
    for(int i = 0; i < rows; i++){
        //resets totSales each time so that different products are evaluated
        int totSales = 0;
        for (int j = 0; j < cols; j++){
            totSales = totSales + dataArray[i][j];
        }
        //prints out the total sales for each row in sales.txt
        fprintf(f0,"%02d\t%4d\n", i, totSales);
    }
}

void top_overall(int rows, int cols, int dataArray[][cols]) {
    //initializes most popular item and its index
    int mostPopularSale = 0;
    int mostPopularIndex = 0;

    //sums up the elements in each column, and iterates for each of the 50 rows
    for (int i = 0; i < rows; i++) {
        //resets totSales each time so that different products are evaluated
        int totSales = 0;
        for (int j = 0; j < cols; j++) {
            totSales = totSales + dataArray[i][j];
        }
        //if the total sales are greater than the most popular sales at the moment,
        //updates the most popular sales to be equal to that sale,
        //and updates the index of that item to represent the most popular item
        if (totSales > mostPopularSale) {
            mostPopularSale = totSales;
            mostPopularIndex = i;
        }
    }
    //prints out the most popular item by its index
    printf("The most popular item is item %d.\n", mostPopularIndex);
}

