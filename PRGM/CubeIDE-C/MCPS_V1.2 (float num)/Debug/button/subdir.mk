################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../button/button.c 

OBJS += \
./button/button.o 

C_DEPS += \
./button/button.d 


# Each subdirectory must supply rules for building sources it contributes
button/%.o button/%.su button/%.cyclo: ../button/%.c button/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F334x8 -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I"D:/Projects/MCPS/PRGM/MCPS_V1.2 (float num)/button" -I"D:/Projects/MCPS/PRGM/MCPS_V1.2 (float num)/EEPROM" -I"D:/Projects/MCPS/PRGM/MCPS_V1.2 (float num)/LCDLib" -I"D:/Projects/MCPS/PRGM/MCPS_V1.2 (float num)/Menu" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-button

clean-button:
	-$(RM) ./button/button.cyclo ./button/button.d ./button/button.o ./button/button.su

.PHONY: clean-button

