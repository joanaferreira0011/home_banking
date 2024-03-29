#include "queue.h"

struct Queue *createQueue(unsigned capacity)
{
    struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1; // This is important, see the enqueue
    queue->array = (tlv_request_t *)malloc(queue->capacity * sizeof(tlv_request_t));
    return queue;
}

bool isFull(struct Queue *queue)
{
    return (queue->size == queue->capacity);
}

bool isEmpty(struct Queue *queue)
{
    return (queue->size == 0);
}

void push(struct Queue *queue, tlv_request_t item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

tlv_request_t pop(struct Queue *queue)
{
    // if (isEmpty(queue))
    //     return NULL;

    tlv_request_t item = queue->array[queue->front];

    queue->front = (queue->front + 1) % queue->capacity;

    queue->size = queue->size - 1;


    return item;
}

tlv_request_t front(struct Queue *queue)
{
    // if (isEmpty(queue))
    //     return NULL;
    return queue->array[queue->front];
}

tlv_request_t back(struct Queue *queue)
{
    // if (isEmpty(queue))
    //     return NULL;
    return queue->array[queue->rear];
}
