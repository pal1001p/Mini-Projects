/*
 * Shop_queue.h
 * ------------------------------------------
 * Copyright (c) 2024
 * Yu-Shan Sun
 * CS 2303 (Spring 2024 C-Term)
 * Worcester Polytechnic Institute - CS
 * ------------------------------------------
 */
#ifndef CS2303_C24_ShopQUEUE_H
#define CS2303_C24_ShopQUEUE_H


#include <memory>
#include "customer.h"

using namespace std;

struct node {
  shared_ptr<Customer> data;
  shared_ptr<struct node> next;
};
typedef struct node Node;


class ShopQueue {
  public:
    ShopQueue();
    void addNewCustomer(int, int, int, int);
    int getLength();
    bool isEmpty();
    shared_ptr<Customer> peekFrontCustomer();
    shared_ptr<Customer> removeFrontCustomer();
    int numItems();
  private:
    shared_ptr<Node> front;
    shared_ptr<Node> back;
    int length;
};

#endif // CS2303_C24_SHOPQUEUE_H