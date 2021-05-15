################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/generales.c \
../src/protocolos_comunicacion.c \
../src/sockets.c 

OBJS += \
./src/generales.o \
./src/protocolos_comunicacion.o \
./src/sockets.o 

C_DEPS += \
./src/generales.d \
./src/protocolos_comunicacion.d \
./src/sockets.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


