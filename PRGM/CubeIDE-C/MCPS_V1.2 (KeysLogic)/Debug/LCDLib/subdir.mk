################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LCDLib/LCD.c 

OBJS += \
./LCDLib/LCD.o 

C_DEPS += \
./LCDLib/LCD.d 


# Each subdirectory must supply rules for building sources it contributes
LCDLib/%.o LCDLib/%.su LCDLib/%.cyclo: ../LCDLib/%.c LCDLib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F334x8 -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I"D:/Projects/MCPS/PRGM/CubeIDE-C/MCPS_V1.2 (KeysLogic)/button" -I"D:/Projects/MCPS/PRGM/CubeIDE-C/MCPS_V1.2 (KeysLogic)/EEPROM" -I"D:/Projects/MCPS/PRGM/CubeIDE-C/MCPS_V1.2 (KeysLogic)/LCDLib" -I"D:/Projects/MCPS/PRGM/CubeIDE-C/MCPS_V1.2 (KeysLogic)/Menu" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LCDLib

clean-LCDLib:
	-$(RM) ./LCDLib/LCD.cyclo ./LCDLib/LCD.d ./LCDLib/LCD.o ./LCDLib/LCD.su

.PHONY: clean-LCDLib

