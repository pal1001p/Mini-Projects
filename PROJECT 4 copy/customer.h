/*
 * customer.h
 * ------------------------------------------
 * Copyright (c) 2024
 * Yu-Shan Sun
 * CS 2303 (Spring 2024 C-Term)
 * Worcester Polytechnic Institute - CS
 * ------------------------------------------
 */
#ifndef CS2303_C24_CUSTOMER_H
#define CS2303_C24_CUSTOMER_H

struct customer {
  int arrivalTime;
  int startTime;
  int completedTime;
  int ID;
};
typedef struct customer Customer;
#endif // CS2303_C24_CUSTOMER_H