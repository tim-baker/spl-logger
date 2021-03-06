################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Analog.obj: ../Analog.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/tirtos_msp43x_2_20_00_06/products/tidrivers_msp43x_2_20_00_08/packages/ti/drivers" --include_path="C:/ti/xdctools_3_32_00_06_core/packages/xdc" --include_path="C:/dev/spl-logger/firmware/USB_app/FatFs" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/dev/spl-logger/firmware" --include_path="C:/dev/spl-logger/firmware/driverlib/MSP430F5xx_6xx" --include_path="C:/dev/spl-logger/firmware/USB_config" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power="none" --define=__MSP430F5529__ --define=DEPRECATED -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="Analog.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

GPIO.obj: ../GPIO.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/tirtos_msp43x_2_20_00_06/products/tidrivers_msp43x_2_20_00_08/packages/ti/drivers" --include_path="C:/ti/xdctools_3_32_00_06_core/packages/xdc" --include_path="C:/dev/spl-logger/firmware/USB_app/FatFs" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/dev/spl-logger/firmware" --include_path="C:/dev/spl-logger/firmware/driverlib/MSP430F5xx_6xx" --include_path="C:/dev/spl-logger/firmware/USB_config" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power="none" --define=__MSP430F5529__ --define=DEPRECATED -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="GPIO.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTC.obj: ../RTC.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/tirtos_msp43x_2_20_00_06/products/tidrivers_msp43x_2_20_00_08/packages/ti/drivers" --include_path="C:/ti/xdctools_3_32_00_06_core/packages/xdc" --include_path="C:/dev/spl-logger/firmware/USB_app/FatFs" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/dev/spl-logger/firmware" --include_path="C:/dev/spl-logger/firmware/driverlib/MSP430F5xx_6xx" --include_path="C:/dev/spl-logger/firmware/USB_config" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power="none" --define=__MSP430F5529__ --define=DEPRECATED -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="RTC.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

hal.obj: ../hal.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/tirtos_msp43x_2_20_00_06/products/tidrivers_msp43x_2_20_00_08/packages/ti/drivers" --include_path="C:/ti/xdctools_3_32_00_06_core/packages/xdc" --include_path="C:/dev/spl-logger/firmware/USB_app/FatFs" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/dev/spl-logger/firmware" --include_path="C:/dev/spl-logger/firmware/driverlib/MSP430F5xx_6xx" --include_path="C:/dev/spl-logger/firmware/USB_config" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power="none" --define=__MSP430F5529__ --define=DEPRECATED -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="hal.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/tirtos_msp43x_2_20_00_06/products/tidrivers_msp43x_2_20_00_08/packages/ti/drivers" --include_path="C:/ti/xdctools_3_32_00_06_core/packages/xdc" --include_path="C:/dev/spl-logger/firmware/USB_app/FatFs" --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/dev/spl-logger/firmware" --include_path="C:/dev/spl-logger/firmware/driverlib/MSP430F5xx_6xx" --include_path="C:/dev/spl-logger/firmware/USB_config" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power="none" --define=__MSP430F5529__ --define=DEPRECATED -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


