#pragma once

#include <stdexcept>

/**
 * Abstract base class defining the interface for a queue of characters.
 */
class Queue {
public:
    virtual ~Queue() = default;

    /**
     * Checks if the queue is empty.
     *
     * @return true if the queue has no elements, false otherwise.
     */
    virtual bool is_empty() const = 0;

    /**
     * Adds a character to the rear of the queue.
     *
     * @param value The character to add.
     */
    virtual void enqueue(char value) = 0;

    /**
     * Removes and returns the character at the front of the queue.
     *
     * @return The character at the front of the queue.
     * @throws std::runtime_error if the queue is empty.
     */
    virtual char dequeue() = 0;

    /**
     * Returns the character at the front of the queue without removing it.
     *
     * @throws std::runtime_error if the queue is empty.
     */
    virtual char peek() const = 0;
};
