################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Team/Generales.c \
../Team/Team.c 

OBJS += \
./Team/Generales.o \
./Team/Team.o 

C_DEPS += \
./Team/Generales.d \
./Team/Team.d 


# Each subdirectory must supply rules for building sources it contributes
Team/%.o: ../Team/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


