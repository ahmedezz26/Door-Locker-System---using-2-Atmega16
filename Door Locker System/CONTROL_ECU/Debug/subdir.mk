################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ALARM.c \
../CONTROL_ECU.c \
../Motor.c \
../TIMER.c \
../UART.c \
../external_eeprom.c \
../i2c.c 

OBJS += \
./ALARM.o \
./CONTROL_ECU.o \
./Motor.o \
./TIMER.o \
./UART.o \
./external_eeprom.o \
./i2c.o 

C_DEPS += \
./ALARM.d \
./CONTROL_ECU.d \
./Motor.d \
./TIMER.d \
./UART.d \
./external_eeprom.d \
./i2c.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


