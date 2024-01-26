#include "common.h"
#include "../../lib/ff.h"


// u8 bi_usb_rd(void *dst, u32 len) {

//     u8 resp = 0;
//     u16 blen, baddr;

//     while (len) {

//         blen = 512; //rx block len
//         if (blen > len)blen = len;
//         baddr = 512 - blen; //address in fpga internal buffer. requested data length equal to 512-int buffer addr


//         bi_reg_wr(REG_USB_CFG, USB_CMD_RD | baddr); //usb read request. fpga will receive usb bytes until the buffer address reaches 512

//         resp = bi_usb_busy(); //wait until requested data amount will be transferred to the internal buffer
//         if (resp)break; //timeout

//         sysPI_rd(dst, REG_ADDR(REG_USB_DAT + baddr), blen); //get data from internal buffer

//         dst += blen;
//         len -= blen;
//     }

//     return resp;
// }

// u8 bi_usb_can_rd() {

//     u32 status = bi_reg_rd(REG_USB_CFG) & (USB_STA_PWR | USB_STA_RXF);
//     if (status == USB_STA_PWR)return 1;
//     return 0;
// }

// void usbTerminal() {

//     u8 data[4 + 1];
//     u8 tout;

//     gConsPrint("USB COM terminal demo");
//     gConsPrint("Press B to exit");

//     data[4] = 1;

//     while (1) {
//         if (!bi_usb_can_rd())continue;

//         //read from virtual serial port.
//         //size must be a multiple of 4. use 512B blocks for best performance 
//         tout = bi_usb_rd(data, 4);
//         if (tout)continue;

//         //send echo string back to the serial port
//         bi_usb_wr(data, 4);
//     }
// }

// void usb_main(void) {
//     usbTerminal();
// }
