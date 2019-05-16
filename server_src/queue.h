#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

/**
 * @brief Structure to represent a queue
 */
struct Queue
{
    int front, rear, size;
    unsigned capacity;
    int *array;
};

/**
 * @brief Function to create a queue of given capacity, it initializes size of queue as 0.
 * 
 * @return A struct representing the queue
 */
struct Queue *createQueue(unsigned capacity);

/**
 * @brief Funcion that checks if the queue is full
 * 
 * @return True if is full, false otherwise
 */
bool isFull(struct Queue *queue);

/**
 * @brief Funcion that checks if the queue is empty
 * 
 * @return True if is empty, false otherwise
 */
bool isEmpty(struct Queue *queue);

/**
 * @brief Function to add an item to the queue. It changes rear and size.
 * 
 * @return 
 */
void push(struct Queue *queue, int item);

/**
 * @brief Function to remove an item from queue. It changes front and size.
 * 
 * @return Returns the removed item.
 */ 
int pop(struct Queue *queue);

/**
 * @brief Function to get the front of queue.
 * 
 * @return Returns the front item.
 */ 
int front(struct Queue *queue);

/**
 * @brief Function to get the back of queue.
 * 
 * @return Returns the back item.
 */ 
int back(struct Queue *queue);