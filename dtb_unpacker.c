#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include "utils.h"

//#define DTB_MAGIC 0xD00DFEED
#define DTB_MAGIC 0xEDFE0DD0

typedef struct {
    uint32_t dtb_magic;
    uint32_t dtb_size;
} DTB_HEADER;



int main(int argc, char *argv[]){
    // Print Help Message
    if(argc == 1 || strcmp(argv[1], "-h") == 0){
        printf("==========================================\n");
        printf("           Unpack DTB Stack!\n");
        printf("==========================================\n");
        printf("Unpack structure like this:\n");
        printf("--------------\n");
        printf("|            |\n");
        printf("|    DTB0    |\n");
        printf("|            |\n");
        printf("--------------\n");
        printf("|            |\n");
        printf("|    DTB1    |\n");
        printf("|            |\n");
        printf("--------------\n");
        printf("|            |\n");
        printf("|    DTB2    |\n");
        printf("|            |\n");
        printf("--------------\n");
        printf("|            |\n");
        printf("|    ....    |\n");
        printf("|            |\n");
        printf("--------------\n\n");
        printf("Usage:\n");
        printf("\tdtb_unpacker dtbs\n");
        printf("Outputs are:\n");
        printf("\t0.dtb, 1.dtb, etc.\n");
        printf("Tips: You can be regenerated the dtb stack by running\n\tcat *.dtb > dtb_stack \n\n");
        return -1;
    }

    // Parse cmdline args
    FileContent stack_dtb = {.filePath = argv[1]};

    // Initialize DTB index
    uint16_t dtb_count = 0;

    // Get file buffer from disk
    if (!get_file_size(&stack_dtb))
        return -EINVAL;
    stack_dtb.fileBuffer = malloc(stack_dtb.fileSize);
    read_file_content(&stack_dtb);

    printf("Input File: %s \nSize: 0x%08lX\n\n", stack_dtb.filePath, stack_dtb.fileSize);

    // Check File Header Loop
    for(uint64_t address=0; address < stack_dtb.fileSize; address++){
        // Check if it has a valid DTB magic
        DTB_HEADER *header = (DTB_HEADER*)(stack_dtb.fileBuffer+address);
        if (header->dtb_magic == DTB_MAGIC){
            printf("Found DTB at 0x%08lX Size 0x%08X\n", address, __builtin_bswap32(header->dtb_size));
            // Create a new file content, then save buffer into file
            char new_dtb_name[10] = {0};
            sprintf(new_dtb_name, "%d.dtb", dtb_count);
            FileContent single_dtb = {
                .filePath = new_dtb_name,
                .fileBuffer = stack_dtb.fileBuffer,
                .fileSize = __builtin_bswap32(header->dtb_size),
            };

            write_file_content(&single_dtb);
            dtb_count++;
            address += __builtin_bswap32(header->dtb_size)-1;
        }
    }
}

