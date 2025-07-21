################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ClassWrUIntSet_Tests.cpp \
../src/JoesSetTestBench.cpp \
../src/WrappedUnsigned.cpp \
../src/WrappedUnsignedLinkedList.cpp \
../src/WrappedUnsignedLinkedListNode.cpp \
../src/WrappedUnsignedSet.cpp 

CPP_DEPS += \
./src/ClassWrUIntSet_Tests.d \
./src/JoesSetTestBench.d \
./src/WrappedUnsigned.d \
./src/WrappedUnsignedLinkedList.d \
./src/WrappedUnsignedLinkedListNode.d \
./src/WrappedUnsignedSet.d 

OBJS += \
./src/ClassWrUIntSet_Tests.o \
./src/JoesSetTestBench.o \
./src/WrappedUnsigned.o \
./src/WrappedUnsignedLinkedList.o \
./src/WrappedUnsignedLinkedListNode.o \
./src/WrappedUnsignedSet.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++20 -O0 -g3 -Wall -Wextra -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/ClassWrUIntSet_Tests.d ./src/ClassWrUIntSet_Tests.o ./src/JoesSetTestBench.d ./src/JoesSetTestBench.o ./src/WrappedUnsigned.d ./src/WrappedUnsigned.o ./src/WrappedUnsignedLinkedList.d ./src/WrappedUnsignedLinkedList.o ./src/WrappedUnsignedLinkedListNode.d ./src/WrappedUnsignedLinkedListNode.o ./src/WrappedUnsignedSet.d ./src/WrappedUnsignedSet.o

.PHONY: clean-src

