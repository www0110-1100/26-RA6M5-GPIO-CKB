/* generated vector source file - do not edit */
        #include "bsp_api.h"
        /* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
        #if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_NUM_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = sci_uart_rxi_isr, /* SCI5 RXI (Receive data full) */
            [1] = sci_uart_txi_isr, /* SCI5 TXI (Transmit data empty) */
            [2] = sci_uart_tei_isr, /* SCI5 TEI (Transmit end) */
            [3] = sci_uart_eri_isr, /* SCI5 ERI (Receive error) */
            [4] = sci_i2c_txi_isr, /* SCI4 TXI (Transmit data empty) */
            [5] = sci_i2c_tei_isr, /* SCI4 TEI (Transmit end) */
            [6] = canfd_error_isr, /* CAN0 CHERR (Channel  error) */
            [7] = canfd_channel_tx_isr, /* CAN0 TX (Transmit interrupt) */
            [8] = canfd_common_fifo_rx_isr, /* CAN0 COMFRX (Common FIFO receive interrupt) */
            [9] = canfd_error_isr, /* CAN GLERR (Global error) */
            [10] = canfd_rx_fifo_isr, /* CAN RXF (Global receive FIFO interrupt) */
            [11] = canfd_error_isr, /* CAN1 CHERR (Channel  error) */
            [12] = canfd_channel_tx_isr, /* CAN1 TX (Transmit interrupt) */
            [13] = canfd_common_fifo_rx_isr, /* CAN1 COMFRX (Common FIFO receive interrupt) */
            [14] = sci_uart_rxi_isr, /* SCI7 RXI (Receive data full) */
            [15] = sci_uart_txi_isr, /* SCI7 TXI (Transmit data empty) */
            [16] = sci_uart_tei_isr, /* SCI7 TEI (Transmit end) */
            [17] = sci_uart_eri_isr, /* SCI7 ERI (Receive error) */
            [18] = iic_master_rxi_isr, /* IIC2 RXI (Receive data full) */
            [19] = iic_master_txi_isr, /* IIC2 TXI (Transmit data empty) */
            [20] = iic_master_tei_isr, /* IIC2 TEI (Transmit end) */
            [21] = iic_master_eri_isr, /* IIC2 ERI (Transfer error) */
            [22] = spi_rxi_isr, /* SPI1 RXI (Receive buffer full) */
            [23] = spi_tei_isr, /* SPI1 TEI (Transmission complete event) */
            [24] = spi_eri_isr, /* SPI1 ERI (Error) */
            [25] = dmac_int_isr, /* DMAC1 INT (DMAC1 transfer end) */
            [26] = sci_uart_rxi_isr, /* SCI3 RXI (Receive data full) */
            [27] = sci_uart_txi_isr, /* SCI3 TXI (Transmit data empty) */
            [28] = sci_uart_tei_isr, /* SCI3 TEI (Transmit end) */
            [29] = sci_uart_eri_isr, /* SCI3 ERI (Receive error) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_NUM_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_SCI5_RXI,GROUP0), /* SCI5 RXI (Receive data full) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_SCI5_TXI,GROUP1), /* SCI5 TXI (Transmit data empty) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_SCI5_TEI,GROUP2), /* SCI5 TEI (Transmit end) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_SCI5_ERI,GROUP3), /* SCI5 ERI (Receive error) */
            [4] = BSP_PRV_VECT_ENUM(EVENT_SCI4_TXI,GROUP4), /* SCI4 TXI (Transmit data empty) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_SCI4_TEI,GROUP5), /* SCI4 TEI (Transmit end) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_CAN0_CHERR,GROUP6), /* CAN0 CHERR (Channel  error) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_CAN0_TX,GROUP7), /* CAN0 TX (Transmit interrupt) */
            [8] = BSP_PRV_VECT_ENUM(EVENT_CAN0_COMFRX,GROUP0), /* CAN0 COMFRX (Common FIFO receive interrupt) */
            [9] = BSP_PRV_VECT_ENUM(EVENT_CAN_GLERR,GROUP1), /* CAN GLERR (Global error) */
            [10] = BSP_PRV_VECT_ENUM(EVENT_CAN_RXF,GROUP2), /* CAN RXF (Global receive FIFO interrupt) */
            [11] = BSP_PRV_VECT_ENUM(EVENT_CAN1_CHERR,GROUP3), /* CAN1 CHERR (Channel  error) */
            [12] = BSP_PRV_VECT_ENUM(EVENT_CAN1_TX,GROUP4), /* CAN1 TX (Transmit interrupt) */
            [13] = BSP_PRV_VECT_ENUM(EVENT_CAN1_COMFRX,GROUP5), /* CAN1 COMFRX (Common FIFO receive interrupt) */
            [14] = BSP_PRV_VECT_ENUM(EVENT_SCI7_RXI,GROUP6), /* SCI7 RXI (Receive data full) */
            [15] = BSP_PRV_VECT_ENUM(EVENT_SCI7_TXI,GROUP7), /* SCI7 TXI (Transmit data empty) */
            [16] = BSP_PRV_VECT_ENUM(EVENT_SCI7_TEI,GROUP0), /* SCI7 TEI (Transmit end) */
            [17] = BSP_PRV_VECT_ENUM(EVENT_SCI7_ERI,GROUP1), /* SCI7 ERI (Receive error) */
            [18] = BSP_PRV_VECT_ENUM(EVENT_IIC2_RXI,GROUP2), /* IIC2 RXI (Receive data full) */
            [19] = BSP_PRV_VECT_ENUM(EVENT_IIC2_TXI,GROUP3), /* IIC2 TXI (Transmit data empty) */
            [20] = BSP_PRV_VECT_ENUM(EVENT_IIC2_TEI,GROUP4), /* IIC2 TEI (Transmit end) */
            [21] = BSP_PRV_VECT_ENUM(EVENT_IIC2_ERI,GROUP5), /* IIC2 ERI (Transfer error) */
            [22] = BSP_PRV_VECT_ENUM(EVENT_SPI1_RXI,GROUP6), /* SPI1 RXI (Receive buffer full) */
            [23] = BSP_PRV_VECT_ENUM(EVENT_SPI1_TEI,GROUP7), /* SPI1 TEI (Transmission complete event) */
            [24] = BSP_PRV_VECT_ENUM(EVENT_SPI1_ERI,GROUP0), /* SPI1 ERI (Error) */
            [25] = BSP_PRV_VECT_ENUM(EVENT_DMAC1_INT,GROUP1), /* DMAC1 INT (DMAC1 transfer end) */
            [26] = BSP_PRV_VECT_ENUM(EVENT_SCI3_RXI,GROUP2), /* SCI3 RXI (Receive data full) */
            [27] = BSP_PRV_VECT_ENUM(EVENT_SCI3_TXI,GROUP3), /* SCI3 TXI (Transmit data empty) */
            [28] = BSP_PRV_VECT_ENUM(EVENT_SCI3_TEI,GROUP4), /* SCI3 TEI (Transmit end) */
            [29] = BSP_PRV_VECT_ENUM(EVENT_SCI3_ERI,GROUP5), /* SCI3 ERI (Receive error) */
        };
        #endif
        #endif