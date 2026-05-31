/* generated vector header file - do not edit */
        #ifndef VECTOR_DATA_H
        #define VECTOR_DATA_H
        #ifdef __cplusplus
        extern "C" {
        #endif
                /* Number of interrupts allocated */
        #ifndef VECTOR_DATA_IRQ_COUNT
        #define VECTOR_DATA_IRQ_COUNT    (30)
        #endif
        /* ISR prototypes */
        void sci_uart_rxi_isr(void);
        void sci_uart_txi_isr(void);
        void sci_uart_tei_isr(void);
        void sci_uart_eri_isr(void);
        void sci_i2c_txi_isr(void);
        void sci_i2c_tei_isr(void);
        void canfd_error_isr(void);
        void canfd_channel_tx_isr(void);
        void canfd_common_fifo_rx_isr(void);
        void canfd_rx_fifo_isr(void);
        void iic_master_rxi_isr(void);
        void iic_master_txi_isr(void);
        void iic_master_tei_isr(void);
        void iic_master_eri_isr(void);
        void spi_rxi_isr(void);
        void spi_tei_isr(void);
        void spi_eri_isr(void);
        void dmac_int_isr(void);

        /* Vector table allocations */
        #define VECTOR_NUMBER_SCI5_RXI ((IRQn_Type) 0) /* SCI5 RXI (Receive data full) */
        #define SCI5_RXI_IRQn          ((IRQn_Type) 0) /* SCI5 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI5_TXI ((IRQn_Type) 1) /* SCI5 TXI (Transmit data empty) */
        #define SCI5_TXI_IRQn          ((IRQn_Type) 1) /* SCI5 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI5_TEI ((IRQn_Type) 2) /* SCI5 TEI (Transmit end) */
        #define SCI5_TEI_IRQn          ((IRQn_Type) 2) /* SCI5 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI5_ERI ((IRQn_Type) 3) /* SCI5 ERI (Receive error) */
        #define SCI5_ERI_IRQn          ((IRQn_Type) 3) /* SCI5 ERI (Receive error) */
        #define VECTOR_NUMBER_SCI4_TXI ((IRQn_Type) 4) /* SCI4 TXI (Transmit data empty) */
        #define SCI4_TXI_IRQn          ((IRQn_Type) 4) /* SCI4 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI4_TEI ((IRQn_Type) 5) /* SCI4 TEI (Transmit end) */
        #define SCI4_TEI_IRQn          ((IRQn_Type) 5) /* SCI4 TEI (Transmit end) */
        #define VECTOR_NUMBER_CAN0_CHERR ((IRQn_Type) 6) /* CAN0 CHERR (Channel  error) */
        #define CAN0_CHERR_IRQn          ((IRQn_Type) 6) /* CAN0 CHERR (Channel  error) */
        #define VECTOR_NUMBER_CAN0_TX ((IRQn_Type) 7) /* CAN0 TX (Transmit interrupt) */
        #define CAN0_TX_IRQn          ((IRQn_Type) 7) /* CAN0 TX (Transmit interrupt) */
        #define VECTOR_NUMBER_CAN0_COMFRX ((IRQn_Type) 8) /* CAN0 COMFRX (Common FIFO receive interrupt) */
        #define CAN0_COMFRX_IRQn          ((IRQn_Type) 8) /* CAN0 COMFRX (Common FIFO receive interrupt) */
        #define VECTOR_NUMBER_CAN_GLERR ((IRQn_Type) 9) /* CAN GLERR (Global error) */
        #define CAN_GLERR_IRQn          ((IRQn_Type) 9) /* CAN GLERR (Global error) */
        #define VECTOR_NUMBER_CAN_RXF ((IRQn_Type) 10) /* CAN RXF (Global receive FIFO interrupt) */
        #define CAN_RXF_IRQn          ((IRQn_Type) 10) /* CAN RXF (Global receive FIFO interrupt) */
        #define VECTOR_NUMBER_CAN1_CHERR ((IRQn_Type) 11) /* CAN1 CHERR (Channel  error) */
        #define CAN1_CHERR_IRQn          ((IRQn_Type) 11) /* CAN1 CHERR (Channel  error) */
        #define VECTOR_NUMBER_CAN1_TX ((IRQn_Type) 12) /* CAN1 TX (Transmit interrupt) */
        #define CAN1_TX_IRQn          ((IRQn_Type) 12) /* CAN1 TX (Transmit interrupt) */
        #define VECTOR_NUMBER_CAN1_COMFRX ((IRQn_Type) 13) /* CAN1 COMFRX (Common FIFO receive interrupt) */
        #define CAN1_COMFRX_IRQn          ((IRQn_Type) 13) /* CAN1 COMFRX (Common FIFO receive interrupt) */
        #define VECTOR_NUMBER_SCI7_RXI ((IRQn_Type) 14) /* SCI7 RXI (Receive data full) */
        #define SCI7_RXI_IRQn          ((IRQn_Type) 14) /* SCI7 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI7_TXI ((IRQn_Type) 15) /* SCI7 TXI (Transmit data empty) */
        #define SCI7_TXI_IRQn          ((IRQn_Type) 15) /* SCI7 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI7_TEI ((IRQn_Type) 16) /* SCI7 TEI (Transmit end) */
        #define SCI7_TEI_IRQn          ((IRQn_Type) 16) /* SCI7 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI7_ERI ((IRQn_Type) 17) /* SCI7 ERI (Receive error) */
        #define SCI7_ERI_IRQn          ((IRQn_Type) 17) /* SCI7 ERI (Receive error) */
        #define VECTOR_NUMBER_IIC2_RXI ((IRQn_Type) 18) /* IIC2 RXI (Receive data full) */
        #define IIC2_RXI_IRQn          ((IRQn_Type) 18) /* IIC2 RXI (Receive data full) */
        #define VECTOR_NUMBER_IIC2_TXI ((IRQn_Type) 19) /* IIC2 TXI (Transmit data empty) */
        #define IIC2_TXI_IRQn          ((IRQn_Type) 19) /* IIC2 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_IIC2_TEI ((IRQn_Type) 20) /* IIC2 TEI (Transmit end) */
        #define IIC2_TEI_IRQn          ((IRQn_Type) 20) /* IIC2 TEI (Transmit end) */
        #define VECTOR_NUMBER_IIC2_ERI ((IRQn_Type) 21) /* IIC2 ERI (Transfer error) */
        #define IIC2_ERI_IRQn          ((IRQn_Type) 21) /* IIC2 ERI (Transfer error) */
        #define VECTOR_NUMBER_SPI1_RXI ((IRQn_Type) 22) /* SPI1 RXI (Receive buffer full) */
        #define SPI1_RXI_IRQn          ((IRQn_Type) 22) /* SPI1 RXI (Receive buffer full) */
        #define VECTOR_NUMBER_SPI1_TEI ((IRQn_Type) 23) /* SPI1 TEI (Transmission complete event) */
        #define SPI1_TEI_IRQn          ((IRQn_Type) 23) /* SPI1 TEI (Transmission complete event) */
        #define VECTOR_NUMBER_SPI1_ERI ((IRQn_Type) 24) /* SPI1 ERI (Error) */
        #define SPI1_ERI_IRQn          ((IRQn_Type) 24) /* SPI1 ERI (Error) */
        #define VECTOR_NUMBER_DMAC1_INT ((IRQn_Type) 25) /* DMAC1 INT (DMAC1 transfer end) */
        #define DMAC1_INT_IRQn          ((IRQn_Type) 25) /* DMAC1 INT (DMAC1 transfer end) */
        #define VECTOR_NUMBER_SCI3_RXI ((IRQn_Type) 26) /* SCI3 RXI (Receive data full) */
        #define SCI3_RXI_IRQn          ((IRQn_Type) 26) /* SCI3 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI3_TXI ((IRQn_Type) 27) /* SCI3 TXI (Transmit data empty) */
        #define SCI3_TXI_IRQn          ((IRQn_Type) 27) /* SCI3 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI3_TEI ((IRQn_Type) 28) /* SCI3 TEI (Transmit end) */
        #define SCI3_TEI_IRQn          ((IRQn_Type) 28) /* SCI3 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI3_ERI ((IRQn_Type) 29) /* SCI3 ERI (Receive error) */
        #define SCI3_ERI_IRQn          ((IRQn_Type) 29) /* SCI3 ERI (Receive error) */
        /* The number of entries required for the ICU vector table. */
        #define BSP_ICU_VECTOR_NUM_ENTRIES (30)

        #ifdef __cplusplus
        }
        #endif
        #endif /* VECTOR_DATA_H */