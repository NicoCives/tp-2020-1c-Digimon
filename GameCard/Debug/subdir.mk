################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../GameCard.c \
../Inicializacion.c \
../MetodosGC.c 

OBJS += \
./GameCard.o \
./Inicializacion.o \
./MetodosGC.o 

C_DEPS += \
./GameCard.d \
./Inicializacion.d \
./MetodosGC.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


