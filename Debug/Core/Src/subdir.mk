################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/button.c \
../Core/Src/command.c \
../Core/Src/date.c \
../Core/Src/distance.c \
../Core/Src/fuel.c \
../Core/Src/kalman_filter.c \
../Core/Src/keypad.c \
../Core/Src/led.c \
../Core/Src/light.c \
../Core/Src/main.c \
../Core/Src/menu.c \
../Core/Src/oled.c \
../Core/Src/ssd1306.c \
../Core/Src/ssd1306_fonts.c \
../Core/Src/state_machine.c \
../Core/Src/stats.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/temperature.c \
../Core/Src/uart_handle.c \
../Core/Src/uart_system.c 

OBJS += \
./Core/Src/button.o \
./Core/Src/command.o \
./Core/Src/date.o \
./Core/Src/distance.o \
./Core/Src/fuel.o \
./Core/Src/kalman_filter.o \
./Core/Src/keypad.o \
./Core/Src/led.o \
./Core/Src/light.o \
./Core/Src/main.o \
./Core/Src/menu.o \
./Core/Src/oled.o \
./Core/Src/ssd1306.o \
./Core/Src/ssd1306_fonts.o \
./Core/Src/state_machine.o \
./Core/Src/stats.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/temperature.o \
./Core/Src/uart_handle.o \
./Core/Src/uart_system.o 

C_DEPS += \
./Core/Src/button.d \
./Core/Src/command.d \
./Core/Src/date.d \
./Core/Src/distance.d \
./Core/Src/fuel.d \
./Core/Src/kalman_filter.d \
./Core/Src/keypad.d \
./Core/Src/led.d \
./Core/Src/light.d \
./Core/Src/main.d \
./Core/Src/menu.d \
./Core/Src/oled.d \
./Core/Src/ssd1306.d \
./Core/Src/ssd1306_fonts.d \
./Core/Src/state_machine.d \
./Core/Src/stats.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/temperature.d \
./Core/Src/uart_handle.d \
./Core/Src/uart_system.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/main.o: ../Core/Src/main.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -u _printf_float -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/button.cyclo ./Core/Src/button.d ./Core/Src/button.o ./Core/Src/button.su ./Core/Src/command.cyclo ./Core/Src/command.d ./Core/Src/command.o ./Core/Src/command.su ./Core/Src/date.cyclo ./Core/Src/date.d ./Core/Src/date.o ./Core/Src/date.su ./Core/Src/distance.cyclo ./Core/Src/distance.d ./Core/Src/distance.o ./Core/Src/distance.su ./Core/Src/fuel.cyclo ./Core/Src/fuel.d ./Core/Src/fuel.o ./Core/Src/fuel.su ./Core/Src/kalman_filter.cyclo ./Core/Src/kalman_filter.d ./Core/Src/kalman_filter.o ./Core/Src/kalman_filter.su ./Core/Src/keypad.cyclo ./Core/Src/keypad.d ./Core/Src/keypad.o ./Core/Src/keypad.su ./Core/Src/led.cyclo ./Core/Src/led.d ./Core/Src/led.o ./Core/Src/led.su ./Core/Src/light.cyclo ./Core/Src/light.d ./Core/Src/light.o ./Core/Src/light.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/menu.cyclo ./Core/Src/menu.d ./Core/Src/menu.o ./Core/Src/menu.su ./Core/Src/oled.cyclo ./Core/Src/oled.d ./Core/Src/oled.o ./Core/Src/oled.su ./Core/Src/ssd1306.cyclo ./Core/Src/ssd1306.d ./Core/Src/ssd1306.o ./Core/Src/ssd1306.su ./Core/Src/ssd1306_fonts.cyclo ./Core/Src/ssd1306_fonts.d ./Core/Src/ssd1306_fonts.o ./Core/Src/ssd1306_fonts.su ./Core/Src/state_machine.cyclo ./Core/Src/state_machine.d ./Core/Src/state_machine.o ./Core/Src/state_machine.su ./Core/Src/stats.cyclo ./Core/Src/stats.d ./Core/Src/stats.o ./Core/Src/stats.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/temperature.cyclo ./Core/Src/temperature.d ./Core/Src/temperature.o ./Core/Src/temperature.su ./Core/Src/uart_handle.cyclo ./Core/Src/uart_handle.d ./Core/Src/uart_handle.o ./Core/Src/uart_handle.su ./Core/Src/uart_system.cyclo ./Core/Src/uart_system.d ./Core/Src/uart_system.o ./Core/Src/uart_system.su

.PHONY: clean-Core-2f-Src

