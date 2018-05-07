// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#define NAND_CE0        (0xe<<10)
#define NAND_CE1        (0xd<<10)

#define NAND_NCE        0x01
#define NAND_CLE        0x02
#define NAND_ALE        0x04

#define NAND_CTRL_CLE   (NAND_NCE | NAND_CLE)
#define NAND_CTRL_ALE   (NAND_NCE | NAND_ALE)
#define NAND_CTRL_CHANGE        0x80

#define NAND_CMD_READ0          0
#define NAND_CMD_READ1          1
#define NAND_CMD_PAGEPROG       0x10
#define NAND_CMD_READOOB        0x50
#define NAND_CMD_ERASE1         0x60
#define NAND_CMD_STATUS         0x70
#define NAND_CMD_SEQIN          0x80    
#define NAND_CMD_READID         0x90
#define NAND_CMD_ERASE2         0xd0
#define NAND_CMD_RESET          0xff
#define NAND_CMD_NONE           -1

/* Extended commands for large page devices */
#define NAND_CMD_READSTART      0x30

/* Status */
#define NAND_STATUS_FAIL        0x01
#define NAND_STATUS_FAIL_N1     0x02
#define NAND_STATUS_TRUE_READY  0x20
#define NAND_STATUS_READY       0x40
#define NAND_STATUS_WP          0x80

struct nand_timings {
    uint32_t tRC_min;
    uint32_t tREA_max;
    uint32_t RHOH_min;
};

struct nand_chip_table {
    uint8_t             manufacturer_id;
    uint8_t             device_id;    
    const char          *manufacturer_name;
    const char          *device_name;
    struct nand_timings timings;
    /* 
     * extended_id_nand -> pagesize, erase blocksize, OOB size
     * could vary given the same device id.
     */
    bool                extended_id_nand;
    uint64_t            chipsize;               /* MiB */
    /* Valid only if extended_id_nand is false */
    uint32_t            page_size;              /* bytes */
    uint32_t		oobsize;		/* bytes */
    uint32_t            erase_block_size;       /* bytes */
    uint32_t            bus_width;       	/* 8 vs 16 bit */    
};

#define MAX(A, B)               ((A > B) ? A : B)
#define MIN(A, B)               ((A < B) ? A : B)

void nand_command(aml_rawnand_t *rawnand, unsigned int command,
                  int column, int page_addr);
struct nand_chip_table *find_nand_chip_table(uint8_t manuf_id,
                                             uint8_t device_id);
zx_status_t nand_wait(aml_rawnand_t *rawnand, uint32_t timeout_ms);
void nand_command(aml_rawnand_t *rawnand, unsigned int command,
                  int column, int page_addr);
zx_status_t nand_read_page(aml_rawnand_t *rawnand, void *data,
                           void *oob, uint32_t nandpage,
                           int *ecc_correct, int retries);
zx_status_t nand_read_page0(aml_rawnand_t *rawnand, void *data,
                            void *oob, uint32_t nandpage,                                  
                            int *ecc_correct, int retries);
zx_status_t nand_write_page(aml_rawnand_t *rawnand, void *data,
                            void *oob, uint32_t nandpage);
zx_status_t nand_erase_block(aml_rawnand_t *rawnand, uint32_t nandpage);

/*
 * Test functions.
 */
#define AML_RAWNAND_TEST

#ifdef AML_RAWNAND_TEST
void rawnand_read_device_test(aml_rawnand_t* rawnand);
void rawnand_dump_bbt(aml_rawnand_t *rawnand);
void rawnand_test_page0(aml_rawnand_t* rawnand);
void rawnand_erasetest_one_block(aml_rawnand_t* rawnand);
void rawnand_writetest_one_eraseblock(aml_rawnand_t* rawnand,
                                      uint32_t nandpage);
void rawnand_erase_write_test(aml_rawnand_t* rawnand,
                              uint64_t start_byte,
                              uint64_t end_byte);
#endif







