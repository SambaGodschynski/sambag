################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DiscoView.cpp 

OBJS += \
./src/DiscoView.o 

CPP_DEPS += \
./src/DiscoView.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DDISCO_USE_X11 -DBOOST_UBLAS_TYPE_CHECK=0 -I/home/samba/clibs/sambag/src -I/home/samba/clibs/loki/include/ -I/home/samba/clibs/tinyxml/ -I/usr/include/cairo -I/home/samba/clibs/boost -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


