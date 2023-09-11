Communication
=============

LIN stuff

https://github.com/Microchip-MPLAB-Harmony/csp_apps_sam_c20_c21/blob/master/apps/sercom/usart/usart_lin/slave/firmware/src/main.c

https://github.com/Microchip-MPLAB-Harmony/csp_apps_sam_c20_c21/tree/master/apps/sercom/usart

https://asf.microchip.com/docs/latest/sam0.applications.samba_bootloader.saml21_xplained_pro/html/asfdoc_sam0_sercom_usart_lin_use_case.html


Baudrate calculation
--------------------

.. jupyter-execute::
    :hide-code:

    from IPython.display import Latex

    sercom0_clk = 8000000
    target_baudrate = 19200
    samples_per_bit = 16
    baud_register = 65536 * (1 - samples_per_bit * (target_baudrate / sercom0_clk))
    baud_register_int = round(baud_register)
    actual_baudrate = (sercom0_clk / 16) * (1 - (baud_register_int / 65536))
    baudrate_error = 1 - (target_baudrate / actual_baudrate)

    Latex("\\begin{gathered}"
          f"BAUD = {baud_register_int}\\\\"
          f"f_{{BAUD}} = {actual_baudrate} bps\\\\"
          f"E_{{BAUD}} = {baudrate_error * 100}\\\\"
          "\end{gathered}")
