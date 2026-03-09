#pragma once

#include <stdexcept>

/**
 * Abstract base class defining the interface for a stack of characters.
 */
class Stack {
public:
    virtual ~Stack() = default;

    /**
     * Checks if the stack is empty.
     *
     * @return true if there are no elements in the stack, false otherwise.
     */
    virtual bool is_empty() const = 0;

    /**
     * Adds a character to the top of the stack.
     *
     * @param value The character to be pushed onto the stack.
     */
    virtual void push(char value) = 0;

    /**
     * Removes and returns the character at the top of the stack.
     *
     * @return The character at the top of the stack.
     * @throws std::runtime_error if the stack is empty.
     */
    virtual char pop() = 0;

    /**
     * Retrieves, but does not remove, the character at the top of the stack.
     *
     * @return The character currently at the top of the stack.
     * @throws std::runtime_error if the stack is empty.
     */
    virtual char peek() const = 0;
};
