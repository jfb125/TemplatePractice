################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/MyOrderedSetTestBench.cpp \
../src/TestMyOrderedSet.cpp \
../src/WrappedUnsigned.cpp 

CPP_DEPS += \
./src/MyOrderedSetTestBench.d \
./src/TestMyOrderedSet.d \
./src/WrappedUnsigned.d 

OBJS += \
./src/MyOrderedSetTestBench.o \
./src/TestMyOrderedSet.o \
./src/WrappedUnsigned.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++20 -O0 -g3 -Wall -Wextra -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/MyOrderedSetTestBench.d ./src/MyOrderedSetTestBench.o ./src/TestMyOrderedSet.d ./src/TestMyOrderedSet.o ./src/WrappedUnsigned.d ./src/WrappedUnsigned.o

.PHONY: clean-src

