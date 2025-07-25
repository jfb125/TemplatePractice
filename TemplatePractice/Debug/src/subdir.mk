################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/MyOrderedSetTestBench.cpp \
../src/PlayingCard.cpp \
../src/PlayingCard_test.cpp \
../src/TestMyOrderedSet.cpp 

CPP_DEPS += \
./src/MyOrderedSetTestBench.d \
./src/PlayingCard.d \
./src/PlayingCard_test.d \
./src/TestMyOrderedSet.d 

OBJS += \
./src/MyOrderedSetTestBench.o \
./src/PlayingCard.o \
./src/PlayingCard_test.o \
./src/TestMyOrderedSet.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++20 -O0 -g3 -Wall -Wextra -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/MyOrderedSetTestBench.d ./src/MyOrderedSetTestBench.o ./src/PlayingCard.d ./src/PlayingCard.o ./src/PlayingCard_test.d ./src/PlayingCard_test.o ./src/TestMyOrderedSet.d ./src/TestMyOrderedSet.o

.PHONY: clean-src

