#include <stdio.h>
#include <stdlib.h>
#include "register.h"

int main(void){
    //initializes the user's input to 0 for later choice-making
    int userInput = 0;
    
    //initializes array with 50 rows for each product, 12 columns for each month
    int dataArray[50][12] = {0};

    //reads data from January and records its monthly sales to dataArray
    FILE* f1 = fopen("Jan_2023_data.txt", "r");
    for (int i = 0; i < 50; i++){
        fscanf(f1, "%*d %*s %d", &dataArray[i][0]);
    }
    fclose(f1);

    //reads data from Feburary and records its monthly sales to dataArray
    FILE* f2 = fopen("Feb_2023_data.txt", "r");
    for (int i = 0; i < 50; i++){
        fscanf(f2, "%*d %*s %d", &dataArray[i][1]);
    }
    fclose(f2);

    //reads data from March and records its monthly sales to dataArray
    FILE* f3 = fopen("Mar_2023_data.txt", "r");
    for (int i = 0; i < 50; i++){
        fscanf(f3, "%*d %*s %d", &dataArray[i][2]);
    }
    fclose(f3);

    //reads data from April and records its monthly sales to dataArray
    FILE* f4 = fopen("Apr_2023_data.txt", "r");
    for (int i = 0; i < 50; i++){
        fscanf(f4, "%*d %*s %d", &dataArray[i][3]);
    }
    fclose(f4);

    //reads data from May and records its monthly sales to dataArray
    FILE* f5 = fopen("May_2023_data.txt", "r");
    for (int i = 0; i < 50; i++){
        fscanf(f5, "%*d %*s %d", &dataArray[i][4]);
    }
    fclose(f5);


    //reads data from June and records its monthly sales to dataArray
    FILE* f6 = fopen("Jun_2023_data.txt", "r");
    for (int i = 0; i < 50; i++){
        fscanf(f6, "%*d %*s %d", &dataArray[i][5]);
    }
    fclose(f6);


    //reads data from July and records its monthly sales to dataArray
    FILE* f7 = fopen("Jul_2023_data.txt", "r");
    for (int i = 0; i < 50; i++){
        fscanf(f7, "%*d %*s %d", &dataArray[i][6]);
    }
    fclose(f7);

    //reads data from August and records its monthly sales to dataArray
    FILE* f8 = fopen("Aug_2023_data.txt", "r");
    for (int i = 0; i < 50; i++){
        fscanf(f8, "%*d %*s %d", &dataArray[i][7]);
    }
    fclose(f8);

    //reads data from September and records its monthly sales to dataArray
    FILE* f9 = fopen("Sep_2023_data.txt", "r");
    for (int i = 0; i < 50; i++){
        fscanf(f9, "%*d %*s %d", &dataArray[i][8]);
    }
    fclose(f9);

    //reads data from October and records its monthly sales to dataArray
    FILE* f10 = fopen("Oct_2023_data.txt", "r");
    for (int i = 0; i < 50; i++){
        fscanf(f10, "%*d %*s %d", &dataArray[i][9]);
    }
    fclose(f10);

    //reads data from November and records its monthly sales to dataArray
    FILE* f11 = fopen("Nov_2023_data.txt", "r");
    for (int i = 0; i < 50; i++){
        fscanf(f11, "%*d %*s %d", &dataArray[i][10]);
    }
    fclose(f11);

    //reads data from December and records its monthly sales to dataArray
    FILE* f12 = fopen("Dec_2023_data.txt", "r");
    for (int i = 0; i < 50; i++){
        fscanf(f12, "%*d %*s %d", &dataArray[i][11]);
    }
    fclose(f12);

    //prompts user for actions to run program
    printf("Which program would you like to run?\n" 
        "(1) Output the monthly sales of an item.\n"
		"(2) Calculate overall sales for all items.\n"
		"(3) Find the most popular item.\n");

    //takes in user input and updates userInput
    scanf("%d", &userInput);

    //evaluates each choice for userInput and executes corresponding function
    if(userInput == 1){
        print_item(50, 12, dataArray);
    } else if (userInput == 2){
        total_sales(50, 12, dataArray);
    } else if (userInput == 3){
        top_overall(50, 12, dataArray);
    }
}