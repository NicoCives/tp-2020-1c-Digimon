################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../digiCommons/src/generales.c \
../digiCommons/src/protocolos_comunicacion.c \
../digiCommons/src/sockets.c 

OBJS += \
./digiCommons/src/generales.o \
./digiCommons/src/protocolos_comunicacion.o \
./digiCommons/src/sockets.o 

C_DEPS += \
./digiCommons/src/generales.d \
./digiCommons/src/protocolos_comunicacion.d \
./digiCommons/src/sockets.d 


# Each subdirectory must supply rules for building sources it contributes
digiCommons/src/%.o: ../digiCommons/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


