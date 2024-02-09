#include <shared/VM.h>
#include <shared/Debug.h>
#include <shared/Compiler.h>

VM vm;

/**
 * @brief Runs the Virtual Machine. Executes each instruction in the Chunk.
 * @return The result of running the Virtual Machine.
 */
static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op)     \
    do {                  \
        double b = pop(); \
        double a = pop(); \
        push(a op b);     \
    } while (false)

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        printf("          ");
        for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
        case OP_CONSTANT: {
            Value constant = READ_CONSTANT();
            push(constant);
            break;
        }
        case OP_ADD: {
            BINARY_OP(+);
            break;
        }
        case OP_SUBTRACT: {
            BINARY_OP(-);
            break;
        }
        case OP_MULTIPLY: {
            BINARY_OP(*);
            break;
        }
        case OP_DIVIDE: {
            BINARY_OP(/);
            break;
        }
        case OP_NEGATE: {
            push(-pop());
            break;
        }
        case OP_RETURN: {
            printValue(pop());
            printf("\n");
            return INTERPRET_OK;
        }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

/// @brief Resets the VM stack by setting the stack top to the bottom of the stack.
static void resetStack() {
    vm.stackTop = vm.stack;
}

void initVM() {
    resetStack();
}

void freeVM() {
}

InterpretResult interpret(const char* source) {
    compile(source);
    return INTERPRET_OK;
}

void push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}